//
// Created by Rosier on 2023-01-01.
//

#ifndef BLOONSTD6SERVER_PROXY_H
#define BLOONSTD6SERVER_PROXY_H

#include "common.h"
#include "Message.h"
#include "MultiplayerServer.h"
#include "Connection.h"
#include "common.h"
#include "dgdata.h"
#include "Request.h"

#include <boost/json.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/config.hpp>

#include <vector>
#include <thread>
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>

namespace json = boost::json;
namespace bio = boost::iostreams;
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

class Proxy {
// Variables
private:
    bool m_multiplayerServer{true};

public:
    net::io_context m_ioc{1};
    std::shared_ptr<MultiplayerServer> m_server;

// Functions
private:

public:
    void run();

};


class ConnectionManager {
// Variables
private:
    Connection m_clientConnection;
    Connection m_serverConnection;

    X509 *m_caCert;
    EVP_PKEY *m_caKey;

    bool m_blockBFB{true};

public:
    Proxy &m_proxy;
    string m_userId;

// Functions
private:
    bool shouldFilterRequest();
    bool shouldFilterResponse();

    template <bool isRequest>
    bool isGzipEncoded(const http::message<isRequest, http::string_body> &msg);

    static string decompressGzip(const string &data);

    bool modifyResponse(const string &target);
    bool customResponse(const string &target);


public:
    ConnectionManager(Proxy &proxy, std::shared_ptr<boost::asio::basic_stream_socket<tcp>> acceptedSocket,
                      X509 *caCert, EVP_PKEY *caKey);

    void run();
};


// ---=== Response ===---
#define SERVER_USER_ID "ffffffffffffffffffffffff"
#define SERVER_USER_CODE "FFFFFFFFF"
#define SERVER_USER_NAME "BloonsTD6Server"

class Response : public Message<http::response<http::string_body>> {
// Variables
private:

public:
    ConnectionManager &m_manager;
    // string m_userId;
    string m_target;

// Functions
private:
    void localMatch();
    void userSearch(const http::request<http::string_body> &request);
    void publicStats();
    void matchmakingGet();
    void matchmakingJoin();

public:
    explicit Response(ConnectionManager &proxy);

    void modifyResponse(const http::request<http::string_body> &request);
    void createCustom(const http::request<http::string_body> &request);

    bool shouldModify(const string &body);
    bool shouldCreateCustom(const string &body);

    void createHttpMessage(const string &contentType);
};

#endif //BLOONSTD6SERVER_PROXY_H
