//
// Created by Rosier on 2023-01-19.
//

#include "LobbyMessage.h"

using namespace LobbyMessages;

// ---=== Player Info ===---
// Private
void PlayerInfo::decode(std::stringstream &data)
{
    json::error_code ec;
    m_jsonData = json::parse(readString<uint32_t>(data), ec).as_object();
}

void PlayerInfo::encode(Player &player)
{
    m_jsonData = player.getInfo();
}

// Public
PlayerInfo::PlayerInfo(const string &data)
{
    std::stringstream dataStream(data);
    decode(dataStream);
}

PlayerInfo::PlayerInfo(std::stringstream &data)
{
    decode(data);
}

PlayerInfo::PlayerInfo()
= default;

string PlayerInfo::getBytes()
{
    string out;
    writeString<uint32_t>(json::serialize(m_jsonData), out);
    return out;
}

// ---=== Lobby Data Message ===---
// Private
void LobbyMessage::decode(std::stringstream &data)
{
    m_peerId = readLE<uint8_t>(data);
    m_messageType = readLE<uint8_t>(data);

    if (m_messageType == 1) { // player info
        m_data = PlayerInfo(data);
    }
}

// Public
LobbyMessage::LobbyMessage(const string &data)
{
    std::stringstream dataStream(data);
    decode(dataStream);
}

LobbyMessage::LobbyMessage(std::stringstream &data)
{
    decode(data);
}

string LobbyMessage::getBytes()
{
    string out;
    out.push_back(static_cast<char>(m_peerId));
    out.push_back(static_cast<char>(m_messageType));
    if (m_messageType == 1) // Player info
    {
        out.append(std::get<PlayerInfo>(m_data).getBytes());
    }
    else if (m_messageType == 0)
    {
        out.append(std::get<ExternalMessage>(m_data).getBytes());
    }

    return out;
}


