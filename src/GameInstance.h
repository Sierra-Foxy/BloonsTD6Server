//
// Created by Rosier on 2023-01-16.
//

#ifndef BLOONSTD6TOOLS_GAMEINSTANCE_H
#define BLOONSTD6TOOLS_GAMEINSTANCE_H

#include "common.h"
#include "Player.h"
#include "GameConfig.h"
#include "Console.h"

#include <thread>
#include <vector>
#include <array>

class MultiplayerServer;

class GameInstance {
// Variables
private:


public:
    int m_botClients{1};
    string m_matchCode;
    std::vector<Player> m_players;
    bool m_inLobbyState{true};

    uint8_t m_maxPlayers;
    coopGameType m_gameType{coopGameType::STANDARD};
    int m_map;
    bool m_goldenBloon;
    bool m_monkeyTeams;
    std::array<string, 3> m_monkeyTeamMonkeys;
    bool m_collectionBonus;
    difficulty m_difficulty;
    gameMode m_gameMode;
    mapDivisions m_division;
    bool m_elite;
    bool m_ranked;
    string m_dailyChallengeId;
    string m_bossId;

    MultiplayerServer* m_server;
    void (MultiplayerServer::* m_changeMapCb)(const GameInstance&){nullptr};

    Console m_console;
    std::thread m_consoleThread;

// Functions
private:

public:
    GameInstance();

    void setMap(int map);
};


#endif //BLOONSTD6TOOLS_GAMEINSTANCE_H
