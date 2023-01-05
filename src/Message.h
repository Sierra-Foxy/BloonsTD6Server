//
// Created by Rosier on 2023-01-02.
//

#ifndef BLOONSTD6SERVER_MESSAGE_H
#define BLOONSTD6SERVER_MESSAGE_H

#include "common.h"
#include "Hmac.h"

#include <boost/json.hpp>
#include <boost/beast/http.hpp>

namespace json = boost::json;
namespace beast = boost::beast;
namespace http = beast::http;

template <class messageType>
class Message {
// Variables
private:

protected:
    json::object m_jsonMessage;
    messageType m_httpMessage;

public:

// Functions
private:

protected:

public:
//    void makeResponse(const string &target, const string &request, const string &response);
//    void makeRequest(const string &nonce);

    void sign();

    virtual void createHttpMessage(const string &contentType);
    messageType &getHttpMessage();
    void setHttpMessage(messageType &message);

    void parseHttpMessage();
};




#endif //BLOONSTD6SERVER_MESSAGE_H
