//
// Created by Rosier on 2023-01-01.
//

#ifndef BLOONSTD6SERVER_PROXY_H
#define BLOONSTD6SERVER_PROXY_H

#include <vector>
#include <thread>

#include "ConnectionManager.h"
#include "common.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/config.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

class Proxy {
// Variables
private:
    net::io_context m_ioc{1};

public:

// Functions
private:

public:
    void run();

};


#endif //BLOONSTD6SERVER_PROXY_H
