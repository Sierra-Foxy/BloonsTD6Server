//
// Created by Rosier on 2023-02-05.
//

#include "GameConfig.h"
#include <cctype>

#include <algorithm>

string getMapString(int map)
{
    auto it = toMapStringTable.find(map);
    if (it != toMapStringTable.end())
    {
        return it->second;
    }
    throw std::runtime_error("Unknown map enum");
}

int getMapEnum(const string& str)
{
    string lowerStr(str);
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    auto it = toMapEnumTable.find(lowerStr);
    if (it != toMapEnumTable.end())
    {
        return it->second;
    }
    throw std::runtime_error("Unknown map string");
}

string getDifficultyString(difficulty dif)
{
    switch (dif)
    {
        case EASY:
            return "Easy";
        case MEDIUM:
            return "Medium";
        case HARD:
            return "Hard";
    }

    throw std::runtime_error("Unknown difficulty enum");
}

difficulty getDifficultyEnum(const string& str)
{
    string lowerStr(str);
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    if (lowerStr == "easy") {
        return EASY;
    }
    else if (lowerStr == "medium") {
        return MEDIUM;
    }
    else if (lowerStr == "hard") {
        return HARD;
    }

    throw std::runtime_error("Unknown difficulty string");
}

string getGameModeString(gameMode gm)
{
    auto it = toGameModeStringTable.find(gm);
    if (it != toGameModeStringTable.end()) {
        return it->second;
    }

    throw std::runtime_error("Unknown gamemode enum");
}

gameMode getGameModeEnum(const string& str)
{
    string lowerStr(str);
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    auto it = toGameModeEnumTable.find(lowerStr);
    if (it != toGameModeEnumTable.end()) {
        return it->second;
    }

    throw std::runtime_error("Unknown gamemode string");
}

string getDivisionString(mapDivisions division)
{
    auto it = toDivisionStringTable.find(division);
    if (it != toDivisionStringTable.end()) {
        return it->second;
    }

    throw std::runtime_error("Unknown division enum");
}

mapDivisions getDivisionEnum(const string& str)
{
    string lowerStr(str);
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    auto it = toDivisionEnumTable.find(lowerStr);
    if (it != toDivisionEnumTable.end())
    {
        return it->second;
    }

    throw std::runtime_error("Unknown division string");
}





