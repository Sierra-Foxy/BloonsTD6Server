//
// Created by Rosier on 2023-01-02.
//

#ifndef BLOONSTD6SERVER_APIENDPOINT_H
#define BLOONSTD6SERVER_APIENDPOINT_H

#include "common.h"

#include <boost/json.hpp>

using namespace boost;

class ApiEndpoint {
// Variables
private:
    json::object m_jsonMessage;

public:

// Functions
private:

public:
    void makeResponse();
    void makeRequest(const string &nonce);

    void sign();
};


#endif //BLOONSTD6SERVER_APIENDPOINT_H
