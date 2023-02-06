//
// Created by Rosier on 2023-01-17.
//

#include "MultiplayerMessages.h"



// ---=== Message Head ===---
// Public
void MessageHead::decode(const string &data)
{
    string magicString;
    magicString.resize(5, 0);
    m_code.resize(4, 0);
    int typeLength;
    std::stringstream dataStream(data);

    dataStream.read(magicString.data(), 5);
    if (magicString != "NKMlt")
    {
        return;
    }
    dataStream.read((char *)&m_version, 1);
    dataStream.read((char *)&m_messageLength, 2);
    dataStream.read((char *)&typeLength, 4);
    dataStream.read(m_code.data(), typeLength);
}

void MessageHead::decode(std::stringstream &data)
{
    string magicString;
    magicString.resize(5, 0);
    int typeLength;

    data.read(magicString.data(), 5);
    if (magicString != "NKMlt")
    {
        return;
    }
    data.read((char *)&m_version, 1);
    data.read((char *)&m_messageLength, 2);
    data.read((char *)&typeLength, 4);
    m_code.resize(typeLength, 0);
    data.read(m_code.data(), typeLength);
}

string MessageHead::getBytes()
{
    string out;
    out.append("NKMlt");
    out.push_back(static_cast<char>(m_version));
    writeLE<uint16_t>(m_messageLength, out);
    writeString<uint32_t>(m_code, out);

    return out;
}

unsigned int MessageHead::getLength()
{
    return 12+m_code.length();
}


// ---=== Join Session Message ===---
// Private
void JSMMessage::decode(std::stringstream &data)
{
    m_id = readLE<uint32_t>(data);
    m_matchId = readString<int32_t>(data);
    m_apiId = readString<int32_t>(data);
}

// Public
JSMMessage::JSMMessage(const string &data)
{
    std::stringstream dataStream(data);
    decode(dataStream);
}


JSMMessage::JSMMessage(std::stringstream &data)
{
    decode(data);
}


// ---=== Join Session Response Message ===---
// Private
void JSRMMessage::decode(std::stringstream &data)
{
    m_success = data.get();
    uint64_t bitList{readLE<uint64_t>(data)};
    uint64_t bitMask{1};
    for (size_t i{0}; i < sizeof(uint64_t) * 8; i++)
    {
        if ((bitMask & bitList) != 0)
        {
            m_playersConnected.push_back(i + 1);
        }
        bitMask <<= 1;
    }
}

// Public
JSRMMessage::JSRMMessage(const string &data) {
    std::stringstream dataStream(data);
    decode(dataStream);
}


JSRMMessage::JSRMMessage(std::stringstream &data)
{
    decode(data);
}

JSRMMessage::JSRMMessage()
= default;

string JSRMMessage::getBytes()
{
    string out;
    out.push_back(static_cast<char>(m_success));
    uint64_t bitList{0};
    for (auto i : m_playersConnected)
    {
        bitList |= 1 << (i - 1);
    }
    writeLE(bitList, out);
    return out;
}


// ---=== Latency Test Message ===---
// Private
void ECHOMessage::decode(std::stringstream &data)
{
    m_timeStamp = readLE<int64_t>(data);
    m_peerId = readLE<uint32_t>(data);
    m_isReply = readLE<uint8_t>(data);
}

// Public
ECHOMessage::ECHOMessage(const string &data)
{
    std::stringstream dataStream(data);
    decode(dataStream);
}

ECHOMessage::ECHOMessage(std::stringstream &data)
{
    decode(data);
}

string ECHOMessage::getBytes()
{
    string out;
    writeLE(m_timeStamp, out);
    writeLE(m_peerId, out);
    writeLE<uint32_t>(m_isReply, out); // Wants a 32 bit bool lmao
    return out;
}


// ---=== Latency Report Message ===---
// Public
ECHRMessage::ECHRMessage(const string &data)
{
    m_latency = readLE<uint64_t>(data);
}


ECHRMessage::ECHRMessage(std::stringstream &data)
{
    m_latency = readLE<uint64_t>(data);
}


// ---=== Game Message ===---
// Private
void GameMessage::decode(std::stringstream &data)
{
    m_peerId = readLE<uint8_t>(data);
    m_messageType = readLE<uint8_t>(data);
}

// Public
GameMessage::GameMessage(std::stringstream &data)
{
    decode(data);
}


// ---=== MultiplayerMessage ===---
MultiplayerMessage::MultiplayerMessage(const string &data, bool isLobby, SocketType parentSock) :
    m_parentSocket{parentSock}
{
    std::stringstream dataStream(data);
    m_head.decode(dataStream);
    if (m_head.m_code == "JSM")
    {
        m_body = messageTypeVariant {JSMMessage(dataStream)};
    }
    else if (m_head.m_code == "JSRM")
    {
        m_body = messageTypeVariant {JSRMMessage(dataStream)};
    }
    else if (m_head.m_code == "ECHO")
    {
        m_body = messageTypeVariant {ECHOMessage(dataStream)};
    }
    else if (m_head.m_code == "ECHR")
    {
        m_body = messageTypeVariant {ECHRMessage(dataStream)};
    }
    else if (m_head.m_code.empty())
    {
        if (isLobby)
        {
            m_body = messageTypeVariant {LobbyMessage(dataStream)};
        }
        else
        {
            m_body = messageTypeVariant {GameMessage(dataStream)};
        }
    }

}

string MultiplayerMessage::getBytes()
{
    string out{};
    if (std::holds_alternative<JSRMMessage>(m_body))
    {
        out.append(std::get<JSRMMessage>(m_body).getBytes());
    }
    else if (std::holds_alternative<ECHOMessage>(m_body))
    {
        out.append(std::get<ECHOMessage>(m_body).getBytes());
    }
    else if (std::holds_alternative<LobbyMessage>(m_body))
    {
        out.append(std::get<LobbyMessage>(m_body).getBytes());
    }
    m_head.m_messageLength = out.length() + 4 + m_head.m_code.length();
    out.insert(0, m_head.getBytes());
    out.append("Fn");
    return out;
}












