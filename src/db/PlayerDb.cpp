//
// Created by Rosier on 2023-02-18.
//

#include "PlayerDb.h"

#include <fstream>

// Private

// Public
void PlayerDb::addPlayerData(const PlayerData& playerData)
{
    for (auto& p : m_players)
    {
        if (p.playerId == playerData.playerId)
        {
            p = playerData;
            return;
        }
    }
    m_players.emplace_back(playerData);
}

void PlayerDb::addPlayerStats(const PlayerStats& playerStats)
{
    for (auto& p : m_players)
    {
        if (p.playerId == playerStats.playerId)
        {
            p.stats = playerStats;
            return;
        }
    }
    auto& newPlayer{m_players.emplace_back()};
    newPlayer.playerId = playerStats.playerId;
    newPlayer.stats = playerStats;
}

PlayerData PlayerDb::getPlayer(const string& playerId)
{
    for (const auto& p : m_players)
    {
        if (p.playerId == playerId)
        {
            return p;
        }
    }
    throw std::runtime_error("No player with that id");
}

void PlayerDb::loadDb(const string& fileDirectory)
{
    std::ifstream file(fileDirectory);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file");
    }

    json::value data;
    file >> data;
    m_players.clear();
    for (auto& i : data.as_object())
    {
        auto& newPlayer{m_players.emplace_back()};
        if (!i.value().at("data").as_object().empty()) {
            newPlayer = json::value_to<PlayerData>(i.value().at("data"));
        }
        if (!i.value().at("stats").as_object().empty()) {
            newPlayer.stats = json::value_to<PlayerStats>(i.value().at("stats"));
            newPlayer.playerId = newPlayer.stats.playerId;
        }
    }
}

void PlayerDb::saveDb(const string& fileDirectory)
{
    std::ofstream file(fileDirectory);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open/create file");
    }

    json::object data;
    for (const auto& player : m_players)
    {
        data[player.playerId].emplace_object();
        if (player.stats.filled)
        {
            data[player.playerId].as_object()["stats"] = json::value_from(player.stats);
        } else {
            data[player.playerId].as_object()["stats"].emplace_object();
        }

        if (player.filled) {
            data[player.playerId].as_object()["data"] = json::value_from(player);
        } else {
            data[player.playerId].as_object()["data"].emplace_object();
        }
    }

    file << data;
}
