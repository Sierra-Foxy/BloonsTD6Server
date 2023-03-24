//
// Created by Rosier on 2023-02-27.
//

#include "api_proxy.hpp"

#include <openssl/evp.h>

#include <fstream>
#include <cstdio>

using namespace server;
using namespace server::proxy;
using namespace std::string_literals;

ApiProxy::ApiProxy() {
    Ctx_.set_options(boost::asio::ssl::context::default_workarounds |
                    boost::asio::ssl::context::no_sslv2);


    std::ifstream CaCertWithKey(Config_.getCaCertificateLocation() +
                                std::string(".pem"));
    if (!CaCertWithKey.is_open()) { // Create CA certificate and key.
        CaCertWithKey.close();
        createCaCert(Config_.getCaCertificateLocation());
    } else {
        CaCertWithKey.close();
    }

    Ctx_.use_certificate_file(Config_.getCaCertificateLocation() + ".pem"s,
                              ssl::context::file_format::pem);
    Ctx_.use_private_key_file(Config_.getCaCertificateLocation() + ".pem"s,
                              ssl::context::file_format::pem);


}

void ApiProxy::run() {
    Listener_ = std::make_shared<Listener>(Ioc_, Ctx_,
                                           tcp::endpoint{Config_.getAddress(),
                                                         Config_.getPort()},
                                           NkEndpoints_);

    Listener_->run();
    Ioc_.run();
}

void ApiProxy::createCaCert(const std::string& FileLocation) {
    CaCert = X509_new();
    CaKey = EVP_RSA_gen(2048);

    X509_set_version(CaCert, X509_VERSION_3);
    ASN1_INTEGER_set(X509_get_serialNumber(CaCert), 1);

    X509_gmtime_adj(X509_getm_notBefore(CaCert), -86400);
    X509_gmtime_adj(X509_getm_notAfter(CaCert), 315576000);

    X509_NAME* CaName = X509_NAME_new();
    X509_NAME_add_entry_by_txt(CaName, "C", MBSTRING_ASC,
                               reinterpret_cast<const unsigned char*>("CA"), -1, -1, 0);
    X509_NAME_add_entry_by_txt(CaName, "O", MBSTRING_ASC,
                               reinterpret_cast<const unsigned char*>("BloonsTools"), -1, -1, 0);
    X509_NAME_add_entry_by_txt(CaName, "CN", MBSTRING_ASC,
                               reinterpret_cast<const unsigned char*>("BloonsTools"), -1, -1, 0);

    X509_set_subject_name(CaCert, CaName);
    X509_set_issuer_name(CaCert, CaName);

    X509_set_pubkey(CaCert, CaKey);

    X509_sign(CaCert, CaKey, EVP_sha256());

    std::FILE* CaCertAndKeyFile = std::fopen((FileLocation + std::string(".pem")).c_str(), "a");
    if (!CaCertAndKeyFile) {
        std::perror("fopen");
    }
    PEM_write_X509(CaCertAndKeyFile, CaCert);
    PEM_write_PrivateKey(CaCertAndKeyFile, CaKey, nullptr, nullptr, 0, nullptr, nullptr);
    std::fclose(CaCertAndKeyFile);

    std::FILE* CaCertFile = std::fopen((FileLocation + std::string("-cert.pem")).c_str(), "a");
    if (!CaCertFile) {
        std::perror("fopen");
    }
    PEM_write_X509(CaCertFile, CaCert);
    std::fclose(CaCertFile);

    X509_free(CaCert);
    EVP_PKEY_free(CaKey);
}
