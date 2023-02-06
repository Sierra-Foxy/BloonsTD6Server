//
// Created by Rosier on 2023-01-16.
//

#ifndef BLOONSTD6TOOLS_PLAYER_H
#define BLOONSTD6TOOLS_PLAYER_H

#include "Towers.h"
#include "Cosmetics.h"
#include "Bloons.h"
#include "common.h"

#include <boost/json.hpp>

#include <cstdint>
#include <vector>
#include <memory>

namespace json = boost::json;

// class ClientConnection;

class Player {
// Variables
private:

public:
    bool m_isConnecting{true};
    bool m_bot{false};
    uint8_t m_id{0};
    string m_apiId;

    string m_name;
    string m_safeName;
    int m_level{};

    std::vector<string> m_enabledMonkeyKnowledge;
    std::vector<string> m_availableHeroes;
    string m_chosenHero;

    std::vector<std::shared_ptr<Cosmetic>> m_cosmetics;

    std::map<string, string> m_namedMonkeys;

    string m_banner{"TeamsBannerDeafult"}; // Has to be misspelled lol

    bool m_autoplaySetting{true};

    // std::shared_ptr<ClientConnection> m_connection;


// Functions
private:

public:
    Player(uint8_t id, string globalId);

    void updateInfo(json::object &json);
    json::object getInfo();

};





#endif //BLOONSTD6TOOLS_PLAYER_H
