//
// Created by Rosier on 2023-02-11.
//

#ifndef BLOONSTD6TOOLS_CONSOLE_H
#define BLOONSTD6TOOLS_CONSOLE_H

#include "common.h"
// #include "GameInstance.h"

#include <sstream>
#include <unordered_map>
#include <memory>

class GameInstance;

class Console;

struct ConsoleCommand {
    // std::unordered_map<string, std::unique_ptr<ConsoleCommand>> m_children;
    string m_usage;
    string m_description;

    string (Console::*func)(std::stringstream&);


    // ConsoleCommand(string usage, string description, );
};


class Console {
// Variables
private:
    std::unordered_map<string, ConsoleCommand> m_mapCommandTable {
        {"set", {"map set <map>", "Set the current map.", &Console::mapSetCmd}},
        {"get", {"map get", "Get the current map.", &Console::mapGetCmd}},
        {"list", {"map list [<difficulty>]", "List maps. Default is list all maps.", &Console::mapListCmd}}
    };

    std::unordered_map<string, ConsoleCommand> m_commandTable {
        {"map", {"map <command>", "Map commands", &Console::mapCmd}}
    };

public:
    GameInstance& m_gameInstance;

// Functions
private:
    string mapSetCmd(std::stringstream& str);
    string mapGetCmd(std::stringstream& str);
    string mapListCmd(std::stringstream& str);

    string mapCmd(std::stringstream& str);

public:
    explicit Console(GameInstance& instance);

    void startConsole();
};


#endif //BLOONSTD6TOOLS_CONSOLE_H
