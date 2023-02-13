//
// Created by Rosier on 2023-02-11.
//

#include "Console.h"
#include "GameConfig.h"
#include "GameInstance.h"

#include <utility>


// ConsoleCommand::ConsoleCommand(string usage, string description)
//     : m_usage{std::move(usage)}, m_description{std::move(description)}
// {
//
// }


string Console::mapSetCmd(std::stringstream& str)
{
    char map[32];
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
    char cmd[32];
    str >> cmd;
    string output = (*this.*m_mapCommandTable.at(cmd).func)(str);
    return output;
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
                cout << c.first << "\t\t" << c.second.m_description << '\n';
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





