//
// Created by Rosier on 2022-12-31.
//

#ifndef BLOONSTD6SERVER_CONNECTIONMANAGER_H
#define BLOONSTD6SERVER_CONNECTIONMANAGER_H

#include <vector>
#include <thread>
#include <iostream>
#include <sstream>

#include "Connection.h"
#include "common.h"
#include "dgdata.h"

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

namespace bio = boost::iostreams;
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

class ConnectionManager {
// Variables
private:
    Connection m_clientConnection;
    Connection m_serverConnection;

    net::io_context &m_ioc;

    X509 *m_caCert;
    EVP_PKEY *m_caKey;

    bool m_blockBFB{true};

public:

// Functions
private:
    bool shouldFilterRequest();
    bool shouldFilterResponse();

    template <bool isRequest>
    bool isGzipEncoded(const http::message<isRequest, http::string_body> &msg);

    static string decompressGzip(const string &data);


public:
    ConnectionManager(net::io_context &ioc, std::shared_ptr<boost::asio::basic_stream_socket<tcp>> acceptedSocket,
                      X509 *caCert, EVP_PKEY *caKey);

    void run();
};


#endif //BLOONSTD6SERVER_CONNECTIONMANAGER_H
