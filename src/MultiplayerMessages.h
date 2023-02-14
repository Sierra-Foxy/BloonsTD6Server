//
// Created by Rosier on 2023-01-17.
//

#ifndef BLOONSTD6TOOLS_MULTIPLAYERMESSAGES_H
#define BLOONSTD6TOOLS_MULTIPLAYERMESSAGES_H

#include "common.h"
#include "MessagePart.h"
#include "GameInstance.h"
// #include "LobbyMessage.h"
#include "Player.h"
#include "Net.h"
#include "GameConfig.h"

#include <boost/json.hpp>

#include <exception>
#include <sstream>
#include <vector>
#include <variant>
#include <utility>

namespace json = boost::json;

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

class MessageHeader : virtual private MessagePart{
// Variables
private:

public:
    uint8_t m_version{1};
    uint16_t m_messageLength{0};
    string m_code;
    std::stringstream m_dataStream;

    SocketType m_fromSocket;

// Functions
private:
    void decode(std::stringstream &data);

protected:
    virtual string getBytes();

public:
    explicit MessageHeader(std::stringstream &data);
    explicit MessageHeader(string code);
    MessageHeader(MessageHeader&& other) noexcept;

    static uint16_t getLength(const string &str);
};


// ---=== ECHO Message ===---
class ECHOMessage : virtual private MessagePart, public MessageHeader {
// Variables
private:

public:
    int64_t m_timeStamp{};
    uint32_t m_peerId{};
    bool m_isReply{};

// Functions
private:
    void decode(std::stringstream &data);

public:
    explicit ECHOMessage(std::stringstream &data);
    explicit ECHOMessage(MessageHeader &&header);

    string getBytes() override;
};


// ---=== ECHR Message ===---
class ECHRMessage : virtual private MessagePart, public MessageHeader {
// Variables
private:

public:
    uint64_t m_latency{};

// Functions
private:
    void decode(std::stringstream &data);

public:
    explicit ECHRMessage(std::stringstream &data);
    explicit ECHRMessage(MessageHeader &&header);
};


// ---=== JSM Message ===---
class JSMMessage : virtual private MessagePart, public MessageHeader {
// Variables
private:

public:
    uint32_t m_id{};
    string m_matchId{};
    string m_apiId{};

// Functions
private:
    void decode(std::stringstream &data);

protected:
    string getBytes() override;

public:
    explicit JSMMessage(std::stringstream &data);
    explicit JSMMessage(MessageHeader &&header);

};

// ---=== JSRM Message ===---
class JSRMMessage : virtual private MessagePart, public MessageHeader {
// Variables
private:

public:
    bool m_success{};
    std::vector<uint8_t> m_playersConnected{};

// Functions
private:
    void decode(std::stringstream &data);

protected:

public:
    explicit JSRMMessage(std::stringstream &data);
    explicit JSRMMessage(MessageHeader &&header);
    JSRMMessage();

    string getBytes() override;
};

// ---=== Lobby Message Header ===---
class LobbyMessageHeader : virtual private MessagePart, public MessageHeader {
// Variables
private:

public:
    uint8_t m_peerId{0};
    enum class lobbyMsgType {
        EXTERNAL,
        PLAYERINFO,
        REQUESTSTART,
        START,
        GAMESETTINGS
    } m_msgType;

// Functions
private:
    void decode(std::stringstream &data);

protected:
    string getBytes() override;

public:
    explicit LobbyMessageHeader(std::stringstream &data);
    explicit LobbyMessageHeader(lobbyMsgType msgType);
    explicit LobbyMessageHeader(MessageHeader &&header);
    LobbyMessageHeader(LobbyMessageHeader &&other) noexcept;
};

// ---=== Player Info Message ===---
class PlayerInfo : virtual private MessagePart, public LobbyMessageHeader {
// Variables
private:

public:
    json::object m_jsonData;

// Functions
private:
    void decode(std::stringstream &data);

public:
    explicit PlayerInfo(std::stringstream &data);
    explicit PlayerInfo(Player &player);
    explicit PlayerInfo(LobbyMessageHeader &&header);

    string getBytes() override;
};


// ---=== External Message Header ===---
class ExternalMessageHeader : virtual private MessagePart, public LobbyMessageHeader {
// Variables
private:

public:
    enum class externalMsgType: uint8_t {
        COOP_GAME_TYPE_CHANGED = 1,
        MAP_CHANGED,
        DIFFICULTY_CHANGED,
        GAME_MODE_CHANGED,
        BOSS_ELITE_CHANGED,
        BOSS_RANKED_CHANGED,
        MAX_PLAYERS_CHANGED,
        DIVISION_CHANGED,
        PLAYER_STATUS_UPDATED,
        PLAYER_STATUS_REQUESTED
    } m_msgType;

// Functions
private:
    void decode(std::stringstream &data);

public:
    explicit ExternalMessageHeader(std::stringstream &data);
    explicit ExternalMessageHeader(externalMsgType msgType);
    explicit ExternalMessageHeader(LobbyMessageHeader &&header);
    ExternalMessageHeader(ExternalMessageHeader &&other) noexcept;

    string getBytes() override;

};


// ---=== Game Type Changed Message ===---
class CoopGameTypeChangedMessage : virtual private MessagePart, public ExternalMessageHeader {
// Variables
private:

public:
    coopGameType m_coopGameType;
    int m_map;
    bool m_goldenBloon;
    bool m_monkeyTeams;
    std::array<string, 3> m_monkeyTeamMonkeys;
    bool m_collectionBonus;
    string m_difficulty;
    string m_gameMode;
    mapDivisions m_division;
    bool m_bossElite;
    bool m_bossRanked;
    string m_dailyChallengeId;
    string m_bossId;

// Functions
private:
    void decode(std::stringstream &data);

public:
    explicit CoopGameTypeChangedMessage(std::stringstream &data);
    explicit CoopGameTypeChangedMessage(const GameInstance &instance);
    explicit CoopGameTypeChangedMessage(ExternalMessageHeader &&header);

    string getBytes() override;
};


class MaxPlayerChangedMessage : virtual private MessagePart, public ExternalMessageHeader {
// Variables
private:

public:
    uint32_t m_maxPlayers{4};

// Functions
private:
    void decode(std::stringstream &data);

public:
    explicit MaxPlayerChangedMessage(std::stringstream &data);
    explicit MaxPlayerChangedMessage(const GameInstance &instance);
    explicit MaxPlayerChangedMessage(ExternalMessageHeader &&header);

    string getBytes() override;
};


class MapChangedMessage : virtual private MessagePart, public ExternalMessageHeader {
// Variables
private:

public:
    int m_map;
    bool m_goldenBloon;
    bool m_monkeyTeams;
    std::array<string, 3> m_monkeyTeamMonkeys;
    bool m_collectionBonus;


// Functions
private:
    void decode(std::stringstream &data);

public:
    explicit MapChangedMessage(std::stringstream& data);
    explicit MapChangedMessage(const GameInstance& instance);
    explicit MapChangedMessage(ExternalMessageHeader&& header);

    string getBytes() override;

};


class DifficultyChangedMessage : virtual private MessagePart, public ExternalMessageHeader {
// Variables
private:

public:
    string m_difficulty;

// Functions
private:
    void decode(std::stringstream &data);

public:
    explicit DifficultyChangedMessage(std::stringstream& data);
    explicit DifficultyChangedMessage(const GameInstance& instance);
    explicit DifficultyChangedMessage(ExternalMessageHeader&& header);

    string getBytes() override;
};


class GameModeChangedMessage : virtual private MessagePart, public ExternalMessageHeader {
// Variables
private:

public:
    string m_gameMode;

// Functions
private:
    void decode(std::stringstream& data);

public:
    explicit GameModeChangedMessage(std::stringstream& data);
    explicit GameModeChangedMessage(const GameInstance& instance);
    explicit GameModeChangedMessage(ExternalMessageHeader&& header);

    string getBytes() override;
};


class DivisionChangedMessage : virtual private MessagePart, public ExternalMessageHeader {
// Variables
private:

public:
    mapDivisions m_division;

// Functions
private:
    void decode(std::stringstream& data);

public:
    explicit DivisionChangedMessage(std::stringstream& data);
    explicit DivisionChangedMessage(const GameInstance& instance);
    explicit DivisionChangedMessage(ExternalMessageHeader&& header);

    string getBytes() override;
};




#endif //BLOONSTD6TOOLS_MULTIPLAYERMESSAGES_H
