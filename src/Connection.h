//
// Created by Rosier on 2022-12-31.
//

#ifndef BLOONSTD6SERVER_CONNECTION_H
#define BLOONSTD6SERVER_CONNECTION_H

#include <random>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/config.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

#include "common.h"

class Connection {
// Variables
private:
    net::io_context &m_ioc;
    ssl::context m_ctx{ssl::context::tlsv12};

    beast::flat_buffer m_buffer;
    http::request<http::string_body> m_request;
    http::response<http::string_body> m_response;

    string m_sni;

public:
    std::shared_ptr<beast::ssl_stream<tcp::socket&>> m_stream;
    std::shared_ptr<tcp::socket> m_socket;


// Functions
private:


public:
    Connection(net::io_context &ioc);

    void createStream();
    void initClient(const tcp::endpoint &endpoint, const string &sni);
    void initServer();

    string getOriginalDest();
    unsignedString getClientHello();
    string getSNI(const unsignedString &clientHello);

    void createCertificate(X509 *caCertificate, EVP_PKEY *caKey, X509 *upstreamCertificate);
    X509 *getCertificate();

    http::response<http::string_body> receiveResponse();
    http::request<http::string_body> receiveRequest();

    void sendMessage(const http::request<http::string_body> &request);
    void sendMessage(const http::response<http::string_body> &response);

};


#endif //BLOONSTD6SERVER_CONNECTION_H
