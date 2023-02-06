//
// Created by Rosier on 2023-01-16.
//

#include "GameInstance.h"



// Private

// Public
GameInstance::GameInstance()
{
    for (int i{0}; i < m_botClients; i++)
    {
        Player &player = m_players.emplace_back(i+1, "ffffffffffffffffffffffff");
        player.m_chosenHero = "Benjamin";
        player.m_name = "Server Bot " + std::to_string(i);
        player.m_safeName = "ServerBot" + std::to_string(i);
        player.m_level = 50;
        player.m_bot = true;
        player.m_availableHeroes.emplace_back("Benjamin");
    }
}

void GameInstance::changeMap(maps map)
{
    m_map = map;
    if (m_changeMapCb) {
        m_changeMapCb();
    }
}