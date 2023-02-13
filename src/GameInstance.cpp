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