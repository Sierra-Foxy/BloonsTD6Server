//
// Created by Rosier on 2023-02-05.
//

#ifndef BLOONSTD6TOOLS_EXTERNALMESSAGE_H
#define BLOONSTD6TOOLS_EXTERNALMESSAGE_H

#include "GameInstance.h"
#include "common.h"
#include "Net.h"
#include "GameConfig.h"
#include "MessagePart.h"

#include <boost/json.hpp>

#include <variant>
#include <sstream>

namespace json = boost::json;

class CoopGameTypeChangedMessage : private MessagePart {
// Variables
private:

public:
    coopGameType m_coopGameType;
    string m_map;
    bool m_goldenBloon;
    bool m_monkeyTeams;
    std::array<string, 3> m_monkeyTeamMonkeys;
    bool m_collectionBonus;
    string m_difficulty;
    string m_gameMode;
    uint32_t m_division;
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
    CoopGameTypeChangedMessage();

    string getBytes();

    int sendMsg(SocketType sock);

};

typedef std::variant<CoopGameTypeChangedMessage> externalMessageVariant;

class ExternalMessage : private MessagePart {
// Variables
private:

public:
    enum dataType: uint8_t {
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
    } m_dataType;
    externalMessageVariant m_data;

// Functions
private:
    void decode(std::stringstream &data);

public:

    string getBytes();

    template<class T>
    explicit ExternalMessage(T msgData) : m_data{msgData}
    {
        if (std::holds_alternative<CoopGameTypeChangedMessage>(m_data))
        {
            m_dataType = COOPGAMETYPECHANGED;
        }
    };
};


#endif //BLOONSTD6TOOLS_EXTERNALMESSAGE_H
