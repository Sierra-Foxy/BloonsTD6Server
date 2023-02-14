//
// Created by Rosier on 2023-02-11.
//

#include "Console.h"
#include "GameConfig.h"
#include "GameInstance.h"

#include <utility>
#include <iomanip>

// ConsoleCommand::ConsoleCommand(string usage, string description)
//     : m_usage{std::move(usage)}, m_description{std::move(description)}
// {
//
// }

// ---=== Map commands ===---
string Console::mapSetCmd(std::stringstream& str)
{
    string map;
    str >> map;
    try {
        m_gameInstance.setMap(getMapEnum(map));
    }
    catch (std::runtime_error& e)
    {
        return "Unknown Map";
    }
    return "";
}

string Console::mapGetCmd(std::stringstream& str)
{
    return getMapString(m_gameInstance.m_map);
}

string Console::mapListCmd(std::stringstream& str)
{
    string difficultyStr;
    str >> difficultyStr;

    int mapMin{0};
    int mapMax{LAST_MAP};
    if (!difficultyStr.empty())
    {
        std::transform(difficultyStr.begin(), difficultyStr.end(), difficultyStr.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        if (difficultyStr == "beginner") {
            mapMax = LAST_BEGINNER;
        }
        else if (difficultyStr == "intermediate") {
            mapMin = FIRST_INTERMEDIATE;
            mapMax = LAST_INTERMEDIATE;
        }
        else if (difficultyStr == "advanced")
        {
            mapMin = FIRST_ADVANCED;
            mapMax = LAST_ADVANCED;
        }
        else if (difficultyStr == "expert")
        {
            mapMin = FIRST_EXPERT;
            mapMax = LAST_EXPERT;
        }
    }

    string out;
    for (int map{mapMin}; map <= mapMax; map++)
    {
        out.append(getMapString(map));
        out.push_back('\n');
    }
    out.pop_back();
    return out;
}

string Console::mapCmd(std::stringstream& str)
{
    string cmd;
    str >> cmd;
    return (*this.*m_mapCommandTable.at(cmd).func)(str);
}


// ---=== Player Commands ===---
string Console::playerMax(std::stringstream& str)
{
    string max;
    str >> max;
    if (max.empty())
    {
        return std::to_string(m_gameInstance.m_maxPlayers);
    }
    else
    {
        try {
            m_gameInstance.setMaxPlayers(stoi(max));
        }
        catch (std::logic_error& e)
        {
            return "Invalid max players";
        }
        return "";
    }
}

string Console::playerCmd(std::stringstream& str)
{
    string cmd;
    str >> cmd;
    return (*this.*m_playerCommandTable.at(cmd).func)(str);
}


// ---=== Difficulty Commands ===---
string Console::difficultySetCmd(std::stringstream& str)
{
    string difficulty;
    str >> difficulty;
    try {
        m_gameInstance.setDifficulty(getDifficultyEnum(difficulty));
    }
    catch (std::runtime_error& e)
    {
        return "Unknown Map";
    }
    return "";
}

string Console::difficultyGetCmd(std::stringstream& str)
{
    return getDifficultyString(m_gameInstance.m_difficulty);
}

string Console::difficultyListCmd(std::stringstream& str)
{
    return "Easy\nMedium\nHard";
}

string Console::difficultyCmd(std::stringstream& str)
{
    string cmd;
    str >> cmd;
    return (*this.*m_difficultyCommandTable.at(cmd).func)(str);
}

// ---=== Game mode commands ===---
string Console::gameModeSetCmd(std::stringstream& str)
{
    string gameMode;
    str >> gameMode;
    try {
        m_gameInstance.setGameMode(getGameModeEnum(gameMode));
    }
    catch (std::runtime_error& e)
    {
        return "Unknown gamemode";
    }
    return "";
}

string Console::gameModeGetCmd(std::stringstream& str)
{
    return getGameModeString(m_gameInstance.m_gameMode);
}

string Console::gameModeListCmd(std::stringstream& str)
{
    string out;
    for (const auto& gameMode : toGameModeStringTable)
    {
        out.append(gameMode.second);
        out.push_back('\n');
    }
    out.append("\nNote: You can use Chimps as an alias for Clicks and ABR as an alias for Alternate Bloons Rounds.");
    return out;
}

string Console::gameModeCmd(std::stringstream& str)
{
    string cmd;
    str >> cmd;
    return (*this.*m_gameModeCommandTable.at(cmd).func)(str);
}


// ---=== Division Commands ===---
string Console::divisionSetCmd(std::stringstream& str)
{
    string division;
    str >> division;

    try {
        m_gameInstance.setDivision(getDivisionEnum(division));
    }
    catch (std::runtime_error& e)
    {
        return "Unknown division";
    }
    return "";
}

string Console::divisionGetCmd(std::stringstream& str)
{
    return getDivisionString(m_gameInstance.m_division);
}

string Console::divisionListCmd(std::stringstream& str)
{
    string out;
    for (const auto& division : toDivisionStringTable)
    {
        out.append(division.second);
        out.push_back('\n');
    }
    out.append("\nNote: Circle is the same as Radioactive and Corner is the same as Stairs");
    return out;
}

string Console::divisionCmd(std::stringstream& str)
{
    string cmd;
    str >> cmd;
    return (*this.*m_divisionCommandTable.at(cmd).func)(str);
}

Console::Console(GameInstance& instance) : m_gameInstance(instance)
{

}

void Console::startConsole()
{
    while (true) {
        char intermediate[100];
        std::cin.getline(intermediate, sizeof(intermediate));
        std::stringstream fullCmd{intermediate};
        char cmd[32];
        fullCmd >> cmd;

        try {
            auto function = m_commandTable.at(cmd).func;
        }
        catch (std::out_of_range& e)
        {
            cout << "Unknown command: " << cmd << "\n\n";
            for (const auto& c : m_commandTable)
            {
                cout << "  " << std::setw(12) << std::left << c.first;
                cout << c.second.m_description << '\n';
            }
            cout << "\nUse \'help <command>\' for info regarding specific commands" << endl;
            continue;
        }
        string output{(*this.*m_commandTable.at(cmd).func)(fullCmd)};
        if (!output.empty())
        {
            cout << output << endl;
        }
    }
}


















