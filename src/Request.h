//
// Created by Rosier on 2023-01-05.
//

#ifndef BLOONSTD6SERVER_REQUEST_H
#define BLOONSTD6SERVER_REQUEST_H

#include "Message.h"

#include <boost/json.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>

namespace json = boost::json;
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

class Request : public Message<http::request<http::string_body>> {
// Variables
private:

public:
    string m_target;

// Functions
private:

public:

};


#endif //BLOONSTD6SERVER_REQUEST_H
