//
// Created by Rosier on 2023-01-01.
//

#include "Proxy.h"

// Private

// Public

void Proxy::run()
{
    tcp::acceptor acceptor{m_ioc, {net::ip::make_address("127.0.0.1"), 44333}};

    std::FILE *caCertFile = std::fopen("/home/sierra/Documents/BTD6ServerCA.pem", "r");
    X509 *caCert = PEM_read_X509(caCertFile, nullptr, nullptr, nullptr);
    std::fclose(caCertFile);

    std::FILE *caKeyFile = std::fopen("/home/sierra/Documents/BTD6ServerCAKey.pem", "r");
    EVP_PKEY *caKey = PEM_read_PrivateKey(caKeyFile, nullptr, nullptr, nullptr);
    std::fclose(caKeyFile);

    while(true)
    {
        auto socket {std::make_shared<tcp::socket>(m_ioc)};

        acceptor.accept(*socket);

        auto connection = std::make_shared<ConnectionManager>(m_ioc, socket, caCert, caKey);
        std::thread{&ConnectionManager::run, connection}.detach();
    }
}