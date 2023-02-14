//
// Created by Rosier on 2023-02-11.
//

#ifndef BLOONSTD6TOOLS_CONSOLE_H
#define BLOONSTD6TOOLS_CONSOLE_H

#include "common.h"
// #include "GameInstance.h"

#include <sstream>
#include <unordered_map>
#include <map>
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
    std::map<string, ConsoleCommand> m_mapCommandTable {
        {"set", {"map set <map>", "Set the current map.", &Console::mapSetCmd}},
        {"get", {"map get", "Get the current map.", &Console::mapGetCmd}},
        {"list", {"map list [<difficulty>]", "List maps. Default is list all maps.", &Console::mapListCmd}}
    };

    std::map<string, ConsoleCommand> m_playerCommandTable {
        {"max", {"player max [<count>]", "Sets max player count. If no count is given returns the current max", &Console::playerMax}}
    };

    std::map<string, ConsoleCommand> m_difficultyCommandTable {
        {"set", {"difficulty set <difficulty>", "Set the current difficulty.", &Console::difficultySetCmd}},
        {"get", {"difficulty get", "Get the current difficulty.", &Console::difficultyGetCmd}},
        {"list", {"difficulty list", "Lists all difficulties.", &Console::difficultyListCmd}}
    };

    std::map<string, ConsoleCommand> m_gameModeCommandTable {
        {"set", {"gamemode set <gamemode>", "Set the current gamemode.", &Console::gameModeSetCmd}},
        {"get", {"gamemode get", "Get the current gamemode.", &Console::gameModeGetCmd}},
        {"list", {"gamemode list", "Lists all gamemodes.", &Console::gameModeListCmd}}
    };

    std::map<string, ConsoleCommand> m_divisionCommandTable {
        {"set", {"division set <division>", "Set the current division.", &Console::divisionSetCmd}},
        {"get", {"division get", "Get the current division", &Console::divisionGetCmd}},
        {"list", {"division list", "Lists all divisions", &Console::divisionListCmd}}
    };

    std::map<string, ConsoleCommand> m_commandTable {
        {"map", {"map <command>", "Map commands.", &Console::mapCmd}},
        {"player", {"player <command>", "Player commands.", &Console::playerCmd}},
        {"difficulty", {"difficulty <command>", "Difficulty commands.", &Console::difficultyCmd}},
        {"gamemode", {"gamemode <command>", "Gamemode commands.", &Console::gameModeCmd}},
        {"division", {"division <command>", "Division commands", &Console::divisionCmd}}
    };

public:
    GameInstance& m_gameInstance;

// Functions
private:
    string mapSetCmd(std::stringstream& str);
    string mapGetCmd(std::stringstream& str);
    string mapListCmd(std::stringstream& str);
    string mapCmd(std::stringstream& str);

    string playerMax(std::stringstream& str);
    string playerCmd(std::stringstream& str);

    string difficultySetCmd(std::stringstream& str);
    string difficultyGetCmd(std::stringstream& str);
    string difficultyListCmd(std::stringstream& str);
    string difficultyCmd(std::stringstream& str);

    string gameModeSetCmd(std::stringstream& str);
    string gameModeGetCmd(std::stringstream& str);
    string gameModeListCmd(std::stringstream& str);
    string gameModeCmd(std::stringstream& str);

    string divisionSetCmd(std::stringstream& str);
    string divisionGetCmd(std::stringstream& str);
    string divisionListCmd(std::stringstream& str);
    string divisionCmd(std::stringstream& str);

public:
    explicit Console(GameInstance& instance);

    void startConsole();
};


#endif //BLOONSTD6TOOLS_CONSOLE_H
