//
// Created by Rosier on 2023-01-16.
//

#include "GameInstance.h"



// Private

// Public
GameInstance::GameInstance() : m_console(*this), m_consoleThread(&Console::startConsole, m_console)
{

    for (int i{0}; i < m_botClients; i++)
    {
        Player &player = m_players.emplace_back(i+1, "ffffffffffffffffffffffff");
        // Player &player = m_players.emplace_back(i+1, "638c61427b1a2590bc72514f");
        player.m_chosenHero = "Benjamin";
        player.m_name = "Server Bot " + std::to_string(i);
        player.m_safeName = "ServerBot" + std::to_string(i);
        // player.m_name = "FoxestFox";
        // player.m_safeName = "ValiantBadger36";
        player.m_level = 50;
        player.m_bot = true;
        player.m_availableHeroes.emplace_back("Benjamin");
    }
}

void GameInstance::setMap(int map)
{
    m_map = map;
    if (m_changeMapCb) {
        (*m_server.*m_changeMapCb)(*this);
    }
}

void GameInstance::setMaxPlayers(int max)
{
    m_maxPlayers = max;
    if (m_changeMaxPlayerCb) {
        (*m_server.*m_changeMaxPlayerCb)(*this);
    }
}

void GameInstance::setDifficulty(difficulty difficulty)
{
    m_difficulty = difficulty;
    if (m_changeDifficultyCb) {
        (*m_server.*m_changeDifficultyCb)(*this);
    }
}

void GameInstance::setGameMode(gameMode gameMode)
{
    m_gameMode = gameMode;
    if (m_changeGameModeCb) {
        (*m_server.*m_changeGameModeCb)(*this);
    }
}

void GameInstance::setDivision(mapDivisions division)
{
    m_division = division;
    if (m_changeDivisionCb) {
        (*m_server.*m_changeDivisionCb)(*this);
    }
}
