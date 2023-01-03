//
// Created by Rosier on 2022-12-31.
//

#include "Connection.h"


// Private



// Public
Connection::Connection(net::io_context &ioc) : m_ioc(ioc)
{}

void Connection::createStream()
{
    m_stream = std::make_shared<beast::ssl_stream<tcp::socket&>>(*m_socket, m_ctx);
}

void Connection::initClient(const tcp::endpoint &endpoint, const string &sni)
{
    m_socket = std::make_shared<tcp::socket>(m_ioc);
    m_socket->connect(endpoint);
    createStream();
    SSL_set_tlsext_host_name(m_stream->native_handle(), sni.c_str());
    m_stream->handshake(ssl::stream_base::client);
}

void Connection::initServer()
{
    createStream();
    m_stream->handshake(ssl::stream_base::server);
}

string Connection::getOriginalDest()
{
    struct sockaddr_in originalDest{};
    socklen_t sinSize{sizeof(originalDest)};
    std::memset(&originalDest, 0, sinSize);
    getsockopt(m_socket->native_handle(), SOL_IP, 80, &originalDest, &sinSize);

    char addr[INET_ADDRSTRLEN];
    socklen_t addrLen = INET_ADDRSTRLEN;
    inet_ntop(AF_INET, &(originalDest.sin_addr), addr, addrLen);

    string addressString{addr, addrLen};
    return addressString;
}

unsignedString Connection::getClientHello()
{
    unsigned char data[2048];
    std::size_t bytesRead = m_socket->receive(net::buffer(data), tcp::socket::message_peek);
    return unsignedString{data, bytesRead};
}

string Connection::getSNI(const unsignedString &clientHello)
{
    unsigned int readPos{43};
    readPos += (unsigned int)clientHello[readPos] + 1; // Read Session ID Length
    readPos += (((unsigned int)clientHello[readPos] << 8) | (unsigned int)clientHello[readPos + 1]) + 2; // Read Cipher Suites Length
    readPos += (unsigned int)clientHello[readPos] + 1; // Read Compression Methods Length
    readPos += 2; // Go to first extension type
    while ((((unsigned int)clientHello[readPos] << 8) | (unsigned int)clientHello[readPos + 1]) != 0)
    {
        readPos += 2; // Go to extension length
        readPos += (((unsigned int)clientHello[readPos] << 8) | (unsigned int)clientHello[readPos + 1]) + 2;
    }

    if (readPos >= clientHello.size()) { return string{}; }
    readPos += 7;

    unsigned int sniLength;
    sniLength = ((unsigned int)clientHello[readPos] << 8) | (unsigned int)clientHello[readPos + 1];
    readPos += 2;
    string sniString{clientHello.begin()+readPos, clientHello.begin()+readPos+sniLength};
    return sniString;
}

void Connection::createCertificate(X509 *caCertificate, EVP_PKEY *caKey, X509 *upstreamCertificate) {
    X509 *serverCertificate;
    EVP_PKEY *serverKey;

    serverCertificate = X509_new();
    serverKey = EVP_RSA_gen(2048);

    X509_set_version(serverCertificate, 2);

    std::mt19937 mt{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };
    ASN1_INTEGER_set(X509_get_serialNumber(serverCertificate), mt());

    X509_gmtime_adj(X509_get_notBefore(serverCertificate), -86400L);
    X509_gmtime_adj(X509_get_notAfter(serverCertificate), 31536000L);

    X509_set_subject_name(serverCertificate, X509_get_subject_name(upstreamCertificate));
    X509_set_issuer_name(serverCertificate, X509_get_subject_name(caCertificate));

    X509_set_pubkey(serverCertificate, serverKey);

    int sanIndex = X509_get_ext_by_NID(upstreamCertificate, NID_subject_alt_name, -1);
    X509_add_ext(serverCertificate, X509_get_ext(upstreamCertificate, sanIndex), -1);

    X509_sign(serverCertificate, caKey, EVP_sha256());

    BIO *certBio = BIO_new(BIO_s_mem());
    PEM_write_bio_X509(certBio, serverCertificate);

    BIO *keyBio = BIO_new(BIO_s_mem());
    PEM_write_bio_PrivateKey(keyBio, serverKey, nullptr, nullptr, 0, nullptr, nullptr);

    const char *certBuffer;
    long certLen = BIO_get_mem_data(certBio, &certBuffer);
    m_ctx.use_certificate(net::buffer(certBuffer, certLen), ssl::context::pem);

    const char *keyBuffer;
    long keyLen = BIO_get_mem_data(keyBio, &keyBuffer);
    m_ctx.use_rsa_private_key(net::buffer(keyBuffer, keyLen), ssl::context::pem);
}

X509 *Connection::getCertificate() {
    return SSL_get_peer_certificate(m_stream->native_handle());
}

http::response<http::string_body> Connection::receiveResponse()
{
    http::response<http::string_body> response{};
    http::read(*m_stream, m_buffer, response);
    return response;
}

http::request<http::string_body> Connection::receiveRequest()
{
    beast::error_code ec;
    http::request<http::string_body> request{};
    http::read(*m_stream, m_buffer, request, ec);
    if (ec == http::error::end_of_stream)
    {
        throw std::runtime_error("end of stream");
    }
    return request;
}

void Connection::sendMessage(const http::request<http::string_body> &request)
{
    http::write(*m_stream, request);
}

void Connection::sendMessage(const http::response<http::string_body> &response)
{
    http::write(*m_stream, response);
}