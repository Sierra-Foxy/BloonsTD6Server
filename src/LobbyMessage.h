//
// Created by Rosier on 2023-01-19.
//

#ifndef BLOONSTD6TOOLS_LOBBYMESSAGE_H
#define BLOONSTD6TOOLS_LOBBYMESSAGE_H


#include "common.h"
#include "ExternalMessage.h"
#include "MessagePart.h"
#include "Player.h"

#include <boost/json.hpp>

#include <variant>
#include <sstream>

namespace json = boost::json;

namespace LobbyMessages {

// ---=== Player Info ===---
class PlayerInfo : private MessagePart {
// Variables
public:
    json::object m_jsonData;

// Functions
private:
    void decode(std::stringstream &data);

public:
    void encode(Player &player);
    explicit PlayerInfo(const string &data);
    explicit PlayerInfo(std::stringstream &data);
    PlayerInfo();

    string getBytes();

};

}

typedef std::variant<LobbyMessages::PlayerInfo, ExternalMessage> lobbyMessageVariant;

// ---=== Lobby Data Message ===---
class LobbyMessage : protected MessagePart {
// Variables
public:
    uint8_t m_peerId{0};
    uint8_t m_messageType{0}; // 0 external, 1 player info, 2 request start, 3 start, 4 game settings

    lobbyMessageVariant m_data;

// Functions
private:
    void decode(std::stringstream &data);

public:
    explicit LobbyMessage(const string &data);
    explicit LobbyMessage(std::stringstream &data);

    string getBytes();

    template <class T>
    explicit LobbyMessage(T msgData) : m_data{msgData}
    {
        if (std::holds_alternative<LobbyMessages::PlayerInfo>(m_data))
        {
            m_messageType = 1;
        }
    }
};



#endif //BLOONSTD6TOOLS_LOBBYMESSAGE_H
