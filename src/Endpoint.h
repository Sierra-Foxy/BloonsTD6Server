//
// Created by Rosier on 2023-01-02.
//

#ifndef BLOONSTD6SERVER_ENDPOINT_H
#define BLOONSTD6SERVER_ENDPOINT_H

#include "common.h"
#include "Hmac.h"

#include <boost/json.hpp>

using namespace boost;

class Endpoint {
// Variables
private:
    json::object m_jsonMessage;

public:

// Functions
private:
    void localMatch(const string &dataStr);
    void userSearch(const string &request, const string &response);

public:
    void makeResponse(const string &target, const string &request, const string &response);
    void makeRequest(const string &nonce);

    void sign();

//    static void escapeString(string &input);
//    static void unescapeString(string &input);

    string str();
};


#endif //BLOONSTD6SERVER_ENDPOINT_H
