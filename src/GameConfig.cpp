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
}

string getGameModeString(gameMode gm)
{
    switch (gm)
    {
        case gameMode::STANDARD:
            return "Standard";
    }
}


