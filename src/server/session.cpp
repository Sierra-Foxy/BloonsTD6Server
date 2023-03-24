//
// Created by Rosier on 2023-03-09.
//

#include "session.hpp"

#include <openssl/rsa.h>
#include <openssl/evp.h>

#include <chrono>

using namespace server::proxy;

const http::response<http::empty_body> Session::kAcceptConnectionResponse_ = http::response<http::empty_body>{http::status::ok, 11};

// Public
Session::Session(tcp::socket&& Socket, ssl::context& CaCert,
                 std::map<std::string, tcp::resolver::results_type>& NkEndpoints)
    : CaCertificate_(CaCert),
      ClientStream_(std::move(Socket)),
      Resolver_(ClientStream_.get_executor()),
      NkEndpoints_(NkEndpoints) {

    SSL_CTX_set_tlsext_servername_callback(DownstreamCtx_.native_handle(), &serverNameCallback);
    SSL_CTX_set_tlsext_servername_arg(DownstreamCtx_.native_handle(), this);
}

void Session::run() {
    net::dispatch(
        ClientStream_.get_executor(),
                  beast::bind_front_handler(
                      &Session::onRun,
                      shared_from_this()));
}

// Private
void Session::onRun() {
    // Receive CONNECT request.
    http::async_read(ClientStream_, ClientBuffer_, Request_,
                     beast::bind_front_handler(&Session::onReceiveConnect,
                                               shared_from_this()));

}

void Session::onReceiveConnect(beast::error_code Ec, std::size_t BytesReceived) {
    if (Ec) {
        return beastFail(Ec, "read");
    }
    if (Request_.method() != http::verb::connect) {
        std::cerr << "Did not receive a proxy CONNECT request." << '\n';
        return;
    }

    std::smatch Match;
    std::string Target(Request_.target());

    std::regex_match(Target, Match, kConnectRegex);

    std::string Host(Match[1]);
    int Port{std::stoi(Match[2])};

    doAccept();
}

void Session::doAccept() {
    http::async_write(ClientStream_, kAcceptConnectionResponse_,
                       beast::bind_front_handler(&Session::onAccept,
                                                 shared_from_this()));
}

void Session::onAccept(beast::error_code Ec, std::size_t BytesSent) {
    if (Ec) {
        return beastFail(Ec, "write");
    }
    ClientTlsStream_.emplace(std::move(ClientStream_), DownstreamCtx_);
    ServerTlsStream_.emplace(ClientTlsStream_->get_executor(), UpstreamCtx_);
    doHandshake();
}

void Session::doHandshake() {
    beast::get_lowest_layer(*ClientTlsStream_).expires_after(std::chrono::seconds(30));

    ClientTlsStream_->async_handshake(ssl::stream_base::server,
                                  beast::bind_front_handler(
                                      &Session::onHandshake,
                                      shared_from_this()));
}

void Session::onHandshake(beast::error_code Ec) {
    if (Ec) {
        return beastFail(Ec, "handshake");
    }
    doReadClient();
}

void Session::doReadClient() {
    Request_ = {};
    beast::get_lowest_layer(*ClientTlsStream_).expires_after(std::chrono::seconds(30));

    http::async_read(*ClientTlsStream_, ClientBuffer_, Request_,
                     beast::bind_front_handler(&Session::onReadClient,
                         shared_from_this()));
}

void Session::onReadClient(beast::error_code Ec, std::size_t BytesTransferred) {
    if (Ec == http::error::end_of_stream) {
        std::cout << "Client end of stream" << '\n';
        doCloseServer();
        return doCloseClient();
    }
    if (Ec) {
        return beastFail(Ec, "read client");
    }

    std::cout << "Request:\n" << Request_ << '\n';
    doWriteServer();
}

void Session::doWriteServer() {
    http::async_write(*ServerTlsStream_, Request_, beast::bind_front_handler(
                                                      &Session::onWriteServer,
                                                      shared_from_this()));
}

void Session::onWriteServer(beast::error_code Ec, std::size_t BytesSent) {
    if (Ec) {
        beastFail(Ec, "write server");
    }
    doReadServer();
}

void Session::doReadServer() {
    Response_ = {};

    beast::get_lowest_layer(*ServerTlsStream_).expires_after(std::chrono::seconds(30));

    http::async_read(*ServerTlsStream_, ServerBuffer_, Response_,
                     beast::bind_front_handler(&Session::onReadServer,
                                               shared_from_this()));
}

void Session::onReadServer(beast::error_code Ec, std::size_t BytesReceived) {
    if (Ec == http::error::end_of_stream) {
        std::cout << "Server end of stream" << '\n';
        return doCloseServer();
    }
    if (Ec) {
        return beastFail(Ec, "read server");
    }

    std::cout << "Response:\n" << Response_ << '\n';
    doWriteClient();
}

void Session::doWriteClient() {
    http::async_write(*ClientTlsStream_, Response_, beast::bind_front_handler(
                                                        &Session::onWriteClient,
                                                        shared_from_this()));
}

void Session::onWriteClient(beast::error_code Ec, std::size_t BytesSent) {
    if (Ec) {
        return beastFail(Ec, "write client");
    }
    std::cout << '\n';

    if (!Response_.keep_alive()) {
        std::cout << "Not keepalive" << '\n';
        doCloseServer();
        return doCloseClient();
    }

    doReadClient();
}


void Session::doCloseClient() {
    beast::get_lowest_layer(*ClientTlsStream_).expires_after(std::chrono::seconds(30));

    ClientTlsStream_->async_shutdown(beast::bind_front_handler(&Session::onShutdown,
                                                               shared_from_this()));
}

void Session::doCloseServer() {
    beast::get_lowest_layer(*ServerTlsStream_).expires_after(std::chrono::seconds(30));

    ServerTlsStream_->async_shutdown(beast::bind_front_handler(&Session::onShutdown,
                                                               shared_from_this()));
}

void Session::onShutdown(beast::error_code Ec) {
    if (Ec == ssl::error::stream_truncated) {
        Ec = {};
    }
    if (Ec) {
        return beastFail(Ec, "shutdown");
    }
}


void Session::connectToServer(const std::string& ServerName) {
    beast::error_code Ec;
    std::cout << "Connecting to: " << ServerName << '\n';

    SSL_set_tlsext_host_name(ServerTlsStream_->native_handle(), ServerName.c_str());
    auto Results = Resolver_.resolve(ServerName, "443");

    beast::get_lowest_layer(*ServerTlsStream_).connect(Results);
    ServerTlsStream_->handshake(ssl::stream_base::client, Ec);
    if (Ec) {
        beastFail(Ec, "Connect to upstream server");
    }

    getPeerCertificate();
}

void Session::getPeerCertificate() {
    X509* PeerCert = SSL_get0_peer_certificate(ServerTlsStream_->native_handle());
    generateDummyCert(PeerCert);
}

void Session::generateDummyCert(X509* PeerCert) {
    // SSL_CTX_get0_certificate and SSL_CTX_get0_privatekey are both undocumented
    // as of writing this. They're as simple as they sound but cannot be found in the openssl man pages.
    X509* CaCert = SSL_CTX_get0_certificate(CaCertificate_.native_handle());
    EVP_PKEY* CaKey = SSL_CTX_get0_privatekey(CaCertificate_.native_handle());

    X509* NewCert = X509_new();
    EVP_PKEY* CertKey = EVP_RSA_gen(2048);
    X509_set_version(NewCert, 2);
    ASN1_INTEGER_set(X509_get_serialNumber(NewCert), 1);

    X509_gmtime_adj(X509_getm_notBefore(NewCert), -86400);
    X509_gmtime_adj(X509_getm_notAfter(NewCert), 31536000);

    X509_set_issuer_name(NewCert, X509_get_subject_name(CaCert));
    X509_set_subject_name(NewCert, X509_get_subject_name(PeerCert));

    X509_set_pubkey(NewCert, CertKey);

    // To get the SAN from the peer we need to get its index. -1 in the last field says to search from beginning
    int SubjectAltNameIndex = X509_get_ext_by_NID(PeerCert, NID_subject_alt_name, -1);
    X509_add_ext(NewCert, X509_get_ext(PeerCert, SubjectAltNameIndex), -1); // -1 to append to extension

    X509_sign(NewCert, CaKey, EVP_sha256());

//    SSL_CTX_use_certificate(DownstreamCtx_.native_handle(), NewCert);
//    SSL_CTX_use_PrivateKey(DownstreamCtx_.native_handle(), CertKey);
    SSL_use_certificate(ClientTlsStream_->native_handle(), NewCert);
    SSL_use_PrivateKey(ClientTlsStream_->native_handle(), CertKey);

    X509_free(NewCert);
    EVP_PKEY_free(CertKey);
}

int Session::serverNameCallback(SSL* Ssl, int* Alert, void* Arg) {
    static_cast<Session*>(Arg)->connectToServer(SSL_get_servername(Ssl, TLSEXT_NAMETYPE_host_name));
    return SSL_TLSEXT_ERR_OK;
}

