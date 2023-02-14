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

    uint8_t m_maxPlayers{4};
    coopGameType m_gameType{coopGameType::STANDARD};
    int m_map{TUTORIAL};
    bool m_goldenBloon{false};
    bool m_monkeyTeams{false};
    std::array<string, 3> m_monkeyTeamMonkeys;
    bool m_collectionBonus{false};
    difficulty m_difficulty{EASY};
    gameMode m_gameMode{gameMode::STANDARD};
    mapDivisions m_division{DEFAULT};
    bool m_elite{false};
    bool m_ranked{false};
    string m_dailyChallengeId;
    string m_bossId;

    MultiplayerServer* m_server;
    void (MultiplayerServer::* m_changeMapCb)(const GameInstance&){nullptr};
    void (MultiplayerServer::* m_changeMaxPlayerCb)(const GameInstance&){nullptr};
    void (MultiplayerServer::* m_changeDifficultyCb)(const GameInstance&){nullptr};
    void (MultiplayerServer::* m_changeGameModeCb)(const GameInstance&){nullptr};
    void (MultiplayerServer::* m_changeDivisionCb)(const GameInstance&){nullptr};

    Console m_console;
    std::thread m_consoleThread;

// Functions
private:

public:
    GameInstance();

    void setMap(int map);
    void setMaxPlayers(int max);
    void setDifficulty(difficulty difficulty);
    void setGameMode(gameMode gameMode);
    void setDivision(mapDivisions division);
};


#endif //BLOONSTD6TOOLS_GAMEINSTANCE_H
