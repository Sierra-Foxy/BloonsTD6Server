//
// Created by Rosier on 2023-01-02.
//

#include "ApiEndpoint.h"

// Private


// Public

void ApiEndpoint::makeResponse()
{

}

void ApiEndpoint::makeRequest(const string &nonce)
{
    m_jsonMessage["nonce"] = nonce;
}

void ApiEndpoint::sign() {
    m_jsonMessage["sig"] = "1";
}
