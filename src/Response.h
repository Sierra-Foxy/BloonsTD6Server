//
// Created by Rosier on 2023-01-05.
//

#ifndef BLOONSTD6SERVER_RESPONSE_H
#define BLOONSTD6SERVER_RESPONSE_H

#define SERVER_USER_ID "fffffffff"
#define SERVER_USER_CODE "FFFFFFFFF"
#define SERVER_USER_NAME "BloonsTD6Server"

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

class Response : public Message<http::response<http::string_body>> {
// Variables
private:

public:
    string m_target;

// Functions
private:
    void localMatch();
    void userSearch(const http::request<http::string_body> &request);
    void publicStats();
    void matchmakingGet();
    void matchmakingJoin();

public:
    void modifyResponse(const http::request<http::string_body> &request);
    void createCustom(const http::request<http::string_body> &request);

    bool shouldModify(const string &body);
    bool shouldCreateCustom(const string &body);

    void createHttpMessage(const string &contentType);
};


#endif //BLOONSTD6SERVER_RESPONSE_H
