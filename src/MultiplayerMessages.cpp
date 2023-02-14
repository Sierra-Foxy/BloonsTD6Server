//
// Created by Rosier on 2023-01-17.
//

#include "MultiplayerMessages.h"

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
// Private
void MessageHeader::decode(std::stringstream &data)
{
    char magicString[6]{0};
    data.read(magicString, 5);
    if (std::strcmp(magicString, "NKMlt") == 0)
    {
        m_version = data.get();
        m_messageLength = readLE<uint16_t>(data);
        m_code = readString<uint32_t>(data);
        return;
    }

    throw std::runtime_error("Invalid Message Header");
}

// Protected
string MessageHeader::getBytes()
{
    string out;
    out.append("NKMlt");
    out.push_back(static_cast<char>(m_version));
    m_messageLength += 4 + m_code.length();
    writeLE<uint16_t>(m_messageLength, out);
    writeString<uint32_t>(m_code, out);
    m_messageLength = 0;
    return out;
}

// Public
MessageHeader::MessageHeader(std::stringstream &data) : m_dataStream{std::move(data)}
{
    decode(m_dataStream);
}

MessageHeader::MessageHeader(string code) : m_code{std::move(code)}
{}

MessageHeader::MessageHeader(MessageHeader &&other) noexcept
    : m_version(std::exchange(other.m_version, 0)), m_messageLength(std::exchange(other.m_messageLength, 0)),
      m_code(std::move(other.m_code)), m_dataStream(std::move(other.m_dataStream)),
      m_fromSocket(std::exchange(other.m_fromSocket, 0))
{}

uint16_t MessageHeader::getLength(const string &str)
{
    return std::stoul(str.substr(6, 2));
}


// ---=== ECHOMessage Message ===---
// Private
void ECHOMessage::decode(std::stringstream &data)
{
    m_timeStamp = readLE<int64_t>(data);
    m_peerId = readLE<uint32_t>(data);
    m_isReply = readLE<uint8_t>(data);
}

// Public
ECHOMessage::ECHOMessage(std::stringstream &data) : MessageHeader(data)
{
    decode(m_dataStream);
}

ECHOMessage::ECHOMessage(MessageHeader &&header) : MessageHeader(std::move(header))
{
    decode(m_dataStream);
}

string ECHOMessage::getBytes()
{
    string out;
    writeLE(m_timeStamp, out);
    writeLE(m_peerId, out);
    writeLE<uint32_t>(m_isReply, out); // Wants a 32 bit bool lmao
    m_messageLength += 16;
    out.insert(0, MessageHeader::getBytes());
    out.append("Fn");
    return out;
}

// ---=== ECHR Message ===---
// Private
void ECHRMessage::decode(std::stringstream &data)
{
    m_latency = readLE<uint64_t>(data);
}

// Public
ECHRMessage::ECHRMessage(std::stringstream &data) : MessageHeader(data)
{
    decode(m_dataStream);
}

ECHRMessage::ECHRMessage(MessageHeader &&header) : MessageHeader(std::move(header))
{
    decode(m_dataStream);
}


// ---=== JSM Message ===---
// Private
void JSMMessage::decode(std::stringstream &data)
{
    m_id = readLE<uint32_t>(data);
    m_matchId = readString<int32_t>(data);
    m_apiId = readString<int32_t>(data);
}

// Protected
string JSMMessage::getBytes()
{
    string out;
    writeLE<uint32_t>(m_id, out);
    writeString<uint32_t>(m_matchId, out);
    writeString<uint32_t>(m_apiId, out);
    m_messageLength += 12 + m_matchId.length() + m_apiId.length();
    out.insert(0, MessageHeader::getBytes());
    out.append("Fn");
    return out;
}

// Public
JSMMessage::JSMMessage(std::stringstream &data) : MessageHeader(data)
{
    decode(m_dataStream);
}

JSMMessage::JSMMessage(MessageHeader &&header) : MessageHeader(std::move(header))
{
    decode(m_dataStream);
}


// ---=== JSRM Message ===---
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
JSRMMessage::JSRMMessage(std::stringstream &data) : MessageHeader(data)
{
    decode(m_dataStream);
}

JSRMMessage::JSRMMessage(MessageHeader &&header) : MessageHeader(std::move(header))
{
    decode(m_dataStream);
}

JSRMMessage::JSRMMessage() : MessageHeader("JSRM")
{}

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

    m_messageLength += 9;
    out.insert(0, MessageHeader::getBytes());
    out.append("Fn");
    return out;
}

// ---=== Lobby Message Header ===---
// Private
void LobbyMessageHeader::decode(std::stringstream &data)
{
    m_peerId = readLE<uint8_t>(data);
    m_msgType = static_cast<lobbyMsgType>(readLE<uint8_t>(data));
}

// Protected
string LobbyMessageHeader::getBytes()
{
    string out;
    out.push_back(static_cast<char>(m_peerId));
    out.push_back(static_cast<char>(m_msgType));

    m_messageLength += 2;
    out.insert(0, MessageHeader::getBytes());
    return out;
}

// Public
LobbyMessageHeader::LobbyMessageHeader(std::stringstream &data) : MessageHeader(data)
{
    decode(m_dataStream);
}

LobbyMessageHeader::LobbyMessageHeader(LobbyMessageHeader::lobbyMsgType msgType)
    : MessageHeader(""), m_msgType{msgType}
{}

LobbyMessageHeader::LobbyMessageHeader(MessageHeader &&header) : MessageHeader(std::move(header))
{
    decode(m_dataStream);
}

LobbyMessageHeader::LobbyMessageHeader(LobbyMessageHeader &&other) noexcept
    : m_peerId(std::exchange(other.m_peerId, 0)), m_msgType(std::exchange(other.m_msgType, lobbyMsgType::EXTERNAL)),
      MessageHeader(std::move(other))
{}


// ---=== Player Info Message ===---
// Private
void PlayerInfo::decode(std::stringstream &data)
{
    json::error_code ec;
    m_jsonData = json::parse(readString<uint32_t>(data), ec).as_object();
}

// Public
PlayerInfo::PlayerInfo(std::stringstream &data) : LobbyMessageHeader(data)
{
    decode(m_dataStream);
}

PlayerInfo::PlayerInfo(Player &player) : LobbyMessageHeader(lobbyMsgType::PLAYERINFO)
{
    m_jsonData = player.getInfo();
}

PlayerInfo::PlayerInfo(LobbyMessageHeader &&header) : LobbyMessageHeader(std::move(header))
{
    decode(m_dataStream);
}

string PlayerInfo::getBytes()
{
    string out;
    string jsonString{json::serialize(m_jsonData)};
    writeString<uint32_t>(jsonString, out);
    m_messageLength += 4 + jsonString.length();

    out.insert(0, LobbyMessageHeader::getBytes());
    out.append("Fn");
    return out;
}


// ---=== External Message Header ===---
// Private
void ExternalMessageHeader::decode(std::stringstream &data)
{
    m_msgType = static_cast<externalMsgType>(readLE<uint8_t>(data));
}

// Public
ExternalMessageHeader::ExternalMessageHeader(std::stringstream &data) : LobbyMessageHeader(data)
{
    decode(m_dataStream);
}

ExternalMessageHeader::ExternalMessageHeader(externalMsgType msgType)
    : m_msgType(msgType), LobbyMessageHeader(LobbyMessageHeader::lobbyMsgType::EXTERNAL)
{}

ExternalMessageHeader::ExternalMessageHeader(LobbyMessageHeader &&header) : LobbyMessageHeader(std::move(header))
{
    decode(m_dataStream);
}

ExternalMessageHeader::ExternalMessageHeader(ExternalMessageHeader &&other) noexcept
    : m_msgType(std::exchange(other.m_msgType, ExternalMessageHeader::externalMsgType::COOP_GAME_TYPE_CHANGED)),
      LobbyMessageHeader(std::move(other))
{
    decode(m_dataStream);
}

string ExternalMessageHeader::getBytes()
{
    string out;
    out.push_back(static_cast<char>(m_msgType));
    m_messageLength += 1;
    out.insert(0, LobbyMessageHeader::getBytes());
    return out;
}


// ---=== Game Type Changed Message ===---
// Private
void CoopGameTypeChangedMessage::decode(std::stringstream &data)
{
    m_coopGameType = static_cast<coopGameType>(data.get());
    m_map = getMapEnum(readString<uint8_t>(data));
    m_goldenBloon = data.get();
    m_monkeyTeams = data.get();
    if (m_monkeyTeams)
    {
        for (auto &monkey : m_monkeyTeamMonkeys)
        {
            monkey = readString<uint8_t>(data);
        }
    }
    m_collectionBonus = data.get();
    m_difficulty = readString<uint8_t>(data);
    m_gameMode = readString<uint8_t>(data);
    m_division = static_cast<mapDivisions>(readLE<uint32_t>(data));
    m_bossElite = data.get();
    m_bossRanked = data.get();
    m_dailyChallengeId = readString<uint8_t>(data);
    m_bossId = readString<uint8_t>(data);
}

// Public
CoopGameTypeChangedMessage::CoopGameTypeChangedMessage(std::stringstream &data) : ExternalMessageHeader(data)
{
    decode(m_dataStream);
}

CoopGameTypeChangedMessage::CoopGameTypeChangedMessage(const GameInstance &instance)
    : ExternalMessageHeader(externalMsgType::COOP_GAME_TYPE_CHANGED)
{
    m_coopGameType = instance.m_gameType;
    m_map = instance.m_map;
    m_goldenBloon = instance.m_goldenBloon;
    m_monkeyTeams = instance.m_monkeyTeams;
    m_monkeyTeamMonkeys = instance.m_monkeyTeamMonkeys;
    m_collectionBonus = instance.m_collectionBonus;
    m_difficulty = getDifficultyString(instance.m_difficulty);
    m_gameMode = getGameModeString(instance.m_gameMode);
    m_division = instance.m_division;
    m_bossElite = instance.m_elite;
    m_bossRanked = instance.m_ranked;
    m_dailyChallengeId = instance.m_dailyChallengeId;
    m_bossId = instance.m_bossId;
}

CoopGameTypeChangedMessage::CoopGameTypeChangedMessage(ExternalMessageHeader &&header)
    : ExternalMessageHeader(std::move(header))
{
    decode(m_dataStream);
}

string CoopGameTypeChangedMessage::getBytes()
{
    string out;
    out.push_back(m_coopGameType);
    writeString<uint8_t>(getMapString(m_map), out);
    out.push_back(static_cast<char>(m_goldenBloon));
    out.push_back(static_cast<char>(m_monkeyTeams));
    if (m_monkeyTeams)
    {
        // write monkey teams
        for (auto &monkey : m_monkeyTeamMonkeys)
        {
            writeString<uint8_t>(monkey, out);
            m_messageLength += 1 + monkey.length();
        }
    }
    out.push_back(static_cast<char>(m_collectionBonus));
    writeString<uint8_t>(m_difficulty, out);
    writeString<uint8_t>(m_gameMode, out);
    writeLE<uint32_t>(m_division, out);
    out.push_back(static_cast<char>(m_bossElite));
    out.push_back(static_cast<char>(m_bossRanked));
    writeString<uint8_t>(m_dailyChallengeId, out);
    writeString<uint8_t>(m_bossId, out);
    m_messageLength += 15 + getMapString(m_map).length() + m_difficulty.length() + m_gameMode.length()
            + m_dailyChallengeId.length() + m_bossId.length();

    out.insert(0, ExternalMessageHeader::getBytes());
    out.append("Fn");
    return out;
}


// ---=== Max Player Changed Message ===---
// Private
void MaxPlayerChangedMessage::decode(std::stringstream &data)
{
    m_maxPlayers = readLE<uint32_t>(data);
}

// Public
MaxPlayerChangedMessage::MaxPlayerChangedMessage(std::stringstream &data) : ExternalMessageHeader(data)
{
    decode(m_dataStream);
}

MaxPlayerChangedMessage::MaxPlayerChangedMessage(const GameInstance &instance)
    : ExternalMessageHeader(externalMsgType::MAX_PLAYERS_CHANGED)
{
    m_maxPlayers = instance.m_maxPlayers;
}

MaxPlayerChangedMessage::MaxPlayerChangedMessage(ExternalMessageHeader &&header)
    : ExternalMessageHeader(std::move(header))
{
    decode(m_dataStream);
}

string MaxPlayerChangedMessage::getBytes()
{
    string out;
    writeLE<uint32_t>(m_maxPlayers, out);

    m_messageLength += 4;
    out.insert(0, ExternalMessageHeader::getBytes());
    out.append("Fn");
    return out;
}

// ---=== Map Changed Message ===---
// Private
void MapChangedMessage::decode(std::stringstream &data)
{
    m_map = getMapEnum(readString<uint8_t>(data));
    m_goldenBloon = data.get();
    m_monkeyTeams = data.get();
    if (m_monkeyTeams)
    {
        for (string& monkey : m_monkeyTeamMonkeys)
        {
            monkey = readString<uint8_t>(data);
        }
    }
    m_collectionBonus = data.get();
}


// Public
MapChangedMessage::MapChangedMessage(std::stringstream& data) : ExternalMessageHeader(data)
{
    decode(m_dataStream);
}

MapChangedMessage::MapChangedMessage(const GameInstance& instance)
    : ExternalMessageHeader(externalMsgType::MAP_CHANGED)
{
    m_map = instance.m_map;
    m_goldenBloon = instance.m_goldenBloon;
    m_monkeyTeams = instance.m_monkeyTeams;
    m_monkeyTeamMonkeys = instance.m_monkeyTeamMonkeys;
    m_collectionBonus = instance.m_collectionBonus;
}

MapChangedMessage::MapChangedMessage(ExternalMessageHeader&& header)
    : ExternalMessageHeader(std::move(header))
{
    decode(m_dataStream);
}

string MapChangedMessage::getBytes()
{
    string out;
    writeString<uint8_t>(getMapString(m_map), out);
    m_messageLength += 1 + getMapString(m_map).length();

    out.push_back(static_cast<char>(m_goldenBloon));
    out.push_back(static_cast<char>(m_monkeyTeams));
    if (!m_monkeyTeamMonkeys.empty())
    {
        for (const string& monkey : m_monkeyTeamMonkeys)
        {
            writeString<uint8_t>(monkey, out);
            m_messageLength += 1 + monkey.length();
        }
    }
    out.push_back(static_cast<char>(m_collectionBonus));

    m_messageLength += 3;
    out.insert(0, ExternalMessageHeader::getBytes());
    out.append("Fn");
    return out;
}


// ---=== Difficulty Changed Message ===---
// Private
void DifficultyChangedMessage::decode(std::stringstream& data)
{
    m_difficulty = readString<uint8_t>(data);
}

// Public
DifficultyChangedMessage::DifficultyChangedMessage(std::stringstream& data) : ExternalMessageHeader(data)
{
    decode(m_dataStream);
}

DifficultyChangedMessage::DifficultyChangedMessage(const GameInstance& instance)
    : ExternalMessageHeader(externalMsgType::DIFFICULTY_CHANGED)
{
    m_difficulty = getDifficultyString(instance.m_difficulty);
}

DifficultyChangedMessage::DifficultyChangedMessage(ExternalMessageHeader&& header)
    : ExternalMessageHeader(std::move(header))
{
    decode(m_dataStream);
}

string DifficultyChangedMessage::getBytes()
{
    string out;
    writeString<uint8_t>(m_difficulty, out);

    m_messageLength += 1 + m_difficulty.length();
    out.insert(0, ExternalMessageHeader::getBytes());
    out.append("Fn");
    return out;
}


// ---=== Game Mode Changed Message ===---
// Private
void GameModeChangedMessage::decode(std::stringstream& data)
{
    m_gameMode = readString<uint8_t>(data);
}

// Public
GameModeChangedMessage::GameModeChangedMessage(std::stringstream& data) : ExternalMessageHeader(data)
{
    decode(m_dataStream);
}

GameModeChangedMessage::GameModeChangedMessage(const GameInstance& instance)
    : ExternalMessageHeader(externalMsgType::GAME_MODE_CHANGED)
{
    m_gameMode = getGameModeString(instance.m_gameMode);
}

GameModeChangedMessage::GameModeChangedMessage(ExternalMessageHeader&& header)
    : ExternalMessageHeader(std::move(header))
{
    decode(m_dataStream);
}

string GameModeChangedMessage::getBytes()
{
    string out;
    writeString<uint8_t>(m_gameMode, out);

    m_messageLength += 1 + m_gameMode.length();
    out.insert(0, ExternalMessageHeader::getBytes());
    out.append("Fn");
    return out;
}

// ---=== Division Changed Message ===---
// Private
void DivisionChangedMessage::decode(std::stringstream& data)
{
    m_division = static_cast<mapDivisions>(readLE<uint32_t>(data));
}

// Public
DivisionChangedMessage::DivisionChangedMessage(std::stringstream& data) : ExternalMessageHeader(data)
{
    decode(m_dataStream);
}

DivisionChangedMessage::DivisionChangedMessage(const GameInstance& instance)
    : ExternalMessageHeader(externalMsgType::DIVISION_CHANGED)
{
    m_division = instance.m_division;
}

DivisionChangedMessage::DivisionChangedMessage(ExternalMessageHeader&& header)
    : ExternalMessageHeader(std::move(header))
{
    decode(m_dataStream);
}

string DivisionChangedMessage::getBytes()
{
    string out;
    writeLE<uint32_t>(m_division, out);

    m_messageLength += 4;
    out.insert(0, ExternalMessageHeader::getBytes());
    out.append("Fn");
    return out;
}
















