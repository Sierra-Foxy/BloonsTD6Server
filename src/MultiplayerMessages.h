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

// ---=== Join Session Message ===---
// class JSMMessage : private MessagePart {
// Variables
// public:
//     uint32_t m_id{};
//     string m_matchId{};
//     string m_apiId{};

// Functions
// private:
//     void decode(std::stringstream &data);
//
// public:
//     explicit JSMMessage(const string &data);
//     explicit JSMMessage(std::stringstream &data);
// };


// ---=== Join Session Response Message ===---
// class JSRMMessage : private MessagePart {
// Variables
// public:
//     bool m_success{};
//     std::vector<uint8_t> m_playersConnected{};

// Functions
// private:
//     void decode(std::stringstream &data);
//
// public:
//     explicit JSRMMessage(const string &data);
//     explicit JSRMMessage(std::stringstream &data);
//     JSRMMessage();
//
//     string getBytes();
// };


// ---=== Latency Report Message ===---
// class ECHRMessage : private MessagePart {
// Variables
// public:
//     uint64_t m_latency{};

// Functions
// public:
//     explicit ECHRMessage(const string &data);
//     explicit ECHRMessage(std::stringstream &data);
// };


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




// typedef std::variant<std::monostate, JSMMessage, JSRMMessage, ECHOMessage, ECHRMessage, GameMessage, LobbyMessage> messageTypeVariant;
//
// class MultiplayerMessage{
// // Variables
// private:
//
// public:
//     MessageHead m_head;
//     messageTypeVariant m_body;
//
//     SocketType m_parentSocket{};
//
// // Functions
// private:
//
// public:
//     MultiplayerMessage(const string &data, bool isLobby, SocketType parentSock);
//
//     string getBytes();
//
//     template<class T>
//     explicit MultiplayerMessage(T msgBody) : m_body{msgBody}
//     {
//         if (std::holds_alternative<JSMMessage>(m_body))
//         {
//             m_head.m_code = "JSM";
//         }
//         else if (std::holds_alternative<JSRMMessage>(m_body))
//         {
//             m_head.m_code = "JSRM";
//         }
//         else if (std::holds_alternative<ECHOMessage>(m_body))
//         {
//             m_head.m_code = "ECHOMessage";
//         }
//         else if (std::holds_alternative<ECHRMessage>(m_body))
//         {
//             m_head.m_code = "ECHR";
//         }
//         else if (std::holds_alternative<LobbyMessage>(m_body) || std::holds_alternative<GameMessage>(m_body))
//         {
//             m_head.m_code.clear();
//         }
//     }
// };

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
        COOPGAMETYPECHANGED = 1,
        MAPCHANGED,
        DIFFICULTYCHANGED,
        GAMEMODECHANGED,
        BOSSELITECHANGED,
        BOSSRANKEDCHANGED,
        MAXPLAYERSCHANGED,
        COOPDIVISIONCHANGED,
        PLAYERSTATUSUPDATED,
        PLAYERSTATUSREQUESTED
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




#endif //BLOONSTD6TOOLS_MULTIPLAYERMESSAGES_H
