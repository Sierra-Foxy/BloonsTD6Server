//
// Created by Rosier on 2023-01-17.
//

#ifndef BLOONSTD6TOOLS_MULTIPLAYERMESSAGES_H
#define BLOONSTD6TOOLS_MULTIPLAYERMESSAGES_H

#include "common.h"
#include "MessagePart.h"
#include "LobbyMessage.h"
#include "Net.h"

#include <sstream>
#include <vector>
#include <variant>


// ---=== Message Head ===---
struct MessageHead : private MessagePart {
// Variables
    int m_version{1};
    unsigned int m_messageLength;
    string m_code;

// Functions
    void decode(const string &data);
    void decode(std::stringstream &data);

    string getBytes();
    unsigned int getLength();
};


// ---=== Join Session Message ===---
class JSMMessage : private MessagePart {
// Variables
public:
    uint32_t m_id{};
    string m_matchId{};
    string m_apiId{};

// Functions
private:
    void decode(std::stringstream &data);

public:
    explicit JSMMessage(const string &data);
    explicit JSMMessage(std::stringstream &data);
};


// ---=== Join Session Response Message ===---
class JSRMMessage : private MessagePart {
// Variables
public:
    bool m_success{};
    std::vector<uint8_t> m_playersConnected{};

// Functions
private:
    void decode(std::stringstream &data);

public:
    explicit JSRMMessage(const string &data);
    explicit JSRMMessage(std::stringstream &data);
    JSRMMessage();

    string getBytes();
};


// ---=== Latency Test Message ===---
class ECHOMessage : private MessagePart {
// Variables
public:
    int64_t m_timeStamp{};
    uint32_t m_peerId{};
    bool m_isReply{};

// Functions
private:
    void decode(std::stringstream &data);

public:
    explicit ECHOMessage(const string &data);
    explicit ECHOMessage(std::stringstream &data);

    string getBytes();
};


// ---=== Latency Report Message ===---
class ECHRMessage : private MessagePart {
// Variables
public:
    uint64_t m_latency{};

// Functions
public:
    explicit ECHRMessage(const string &data);
    explicit ECHRMessage(std::stringstream &data);
};


// ---=== Game Data Message ===---
class GameMessage : private MessagePart {
// Variables
public:
    uint8_t m_peerId{};
    uint8_t m_messageType{};

// Functions
private:
    void decode(std::stringstream &data);

public:
    explicit GameMessage(std::stringstream &data);
};




typedef std::variant<std::monostate, JSMMessage, JSRMMessage, ECHOMessage, ECHRMessage, GameMessage, LobbyMessage> messageTypeVariant;

class MultiplayerMessage{
// Variables
private:

public:
    MessageHead m_head;
    messageTypeVariant m_body;

    SocketType m_parentSocket{};

// Functions
private:

public:
    MultiplayerMessage(const string &data, bool isLobby, SocketType parentSock);

    string getBytes();

    template<class T>
    explicit MultiplayerMessage(T msgBody) : m_body{msgBody}
    {
        if (std::holds_alternative<JSMMessage>(m_body))
        {
            m_head.m_code = "JSM";
        }
        else if (std::holds_alternative<JSRMMessage>(m_body))
        {
            m_head.m_code = "JSRM";
        }
        else if (std::holds_alternative<ECHOMessage>(m_body))
        {
            m_head.m_code = "ECHO";
        }
        else if (std::holds_alternative<ECHRMessage>(m_body))
        {
            m_head.m_code = "ECHR";
        }
        else if (std::holds_alternative<LobbyMessage>(m_body) || std::holds_alternative<GameMessage>(m_body))
        {
            m_head.m_code.clear();
        }
    }
};

#endif //BLOONSTD6TOOLS_MULTIPLAYERMESSAGES_H
