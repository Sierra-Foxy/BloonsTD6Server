//
// Created by Rosier on 2023-02-05.
//

#include "ExternalMessage.h"

// CoopGameTypeChangedMessage
// Private
void CoopGameTypeChangedMessage::decode(std::stringstream &data)
{
    m_coopGameType = static_cast<coopGameType>(data.get());
    m_map = readString<uint8_t>(data);
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
    m_division = readLE<uint32_t>(data);
    m_bossElite = data.get();
    m_bossRanked = data.get();
    m_dailyChallengeId = readString<uint8_t>(data);
    m_bossId = readString<uint8_t>(data);
}

// Public
CoopGameTypeChangedMessage::CoopGameTypeChangedMessage(std::stringstream &data)
{
    decode(data);
}

CoopGameTypeChangedMessage::CoopGameTypeChangedMessage(const GameInstance &instance)
{
    m_coopGameType = instance.m_gameType;
    m_map = getMapString(instance.m_map);
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

CoopGameTypeChangedMessage::CoopGameTypeChangedMessage()
= default;

string CoopGameTypeChangedMessage::getBytes()
{
    string out;
    out.push_back(m_coopGameType);
    writeString<uint8_t>(m_map, out);
    out.push_back(static_cast<char>(m_goldenBloon));
    out.push_back(static_cast<char>(m_monkeyTeams));
    if (m_monkeyTeams)
    {
        // write monkey teams
        for (auto &monkey : m_monkeyTeamMonkeys)
        {
            writeString<uint8_t>(monkey, out);
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
    return out;
}

int CoopGameTypeChangedMessage::sendMsg(SocketType sock)
{
}



// Private

// Public
string ExternalMessage::getBytes()
{
    string out;

    out.push_back(m_dataType);

    if (m_dataType == COOPGAMETYPECHANGED)
    {
        out.append(std::get<CoopGameTypeChangedMessage>(m_data).getBytes());
    }

    return out;
}

void ExternalMessage::decode(std::stringstream &data)
{
    m_dataType = static_cast<dataType>(readLE<uint8_t>(data));

    if (m_dataType == COOPGAMETYPECHANGED)
    {
        m_data = CoopGameTypeChangedMessage{data};
    }
}



