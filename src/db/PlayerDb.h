//
// Created by Rosier on 2023-02-18.
//

#ifndef BLOONSTD6TOOLS_PLAYERDB_H
#define BLOONSTD6TOOLS_PLAYERDB_H

#include "common.h"
#include "PlayerData.h"

#include <boost/json.hpp>

#include <vector>
#include <stdexcept>

namespace json = boost::json;


class PlayerDb {
// Variables
private:
    std::vector<PlayerData> m_players;

public:

// Functions
private:

public:
    void addPlayerData(const PlayerData& playerData);
    void addPlayerStats(const PlayerStats& playerStats);

    PlayerData getPlayer(const string& playerId);

    void loadDb(const string& fileDirectory);
    void saveDb(const string& fileDirectory);
};


#endif //BLOONSTD6TOOLS_PLAYERDB_H
