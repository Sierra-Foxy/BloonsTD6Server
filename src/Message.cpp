//
// Created by Rosier on 2023-01-02.
//

#include "Message.h"

// Private


// Public
//template <class messageType>
//void Message<messageType>::makeResponse(const string &target, const string &request, const string &response)
//{
//    if (target == "/matchmaking/find/local")
//    {
//        localMatch(response);
//    }
//    else if (target == "/user/search")
//    {
//        userSearch(request, response);
//    }
//
//    sign();
//    m_jsonMessage["error"] = nullptr;
//}
//
//template <class messageType>
//void Message<messageType>::makeRequest(const string &nonce)
//{
//    m_jsonMessage["nonce"] = nonce;
//}

template <class messageType>
void Message<messageType>::sign() {
    m_jsonMessage["sig"] = hmac(m_jsonMessage["data"].get_string().c_str(), "A26308E242742374");
}

template <class messageType>
void Message<messageType>::createHttpMessage(const string &contentType)
{
    m_httpMessage.body() = json::serialize(m_jsonMessage);
    m_httpMessage.set(http::field::content_type, contentType);
    m_httpMessage.prepare_payload();
}



template <class messageType>
messageType &Message<messageType>::getHttpMessage()
{
    return m_httpMessage;
}

template <class messageType>
void Message<messageType>::setHttpMessage(messageType &message)
{
    json::error_code ec;
    m_httpMessage = std::move(message);
    m_jsonMessage = json::parse(m_httpMessage.body(), ec).as_object();
}

template<class messageType>
void Message<messageType>::parseHttpMessage()
{
    json::error_code ec;
    json::value value = json::parse(m_httpMessage.body(), ec);
    if (ec)
    {
        return;
    }
    else {
        m_jsonMessage = value.as_object();
    }
}












