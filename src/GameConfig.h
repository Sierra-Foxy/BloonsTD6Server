//
// Created by Rosier on 2023-02-05.
//

#ifndef BLOONSTD6TOOLS_GAMECONFIG_H
#define BLOONSTD6TOOLS_GAMECONFIG_H

#include "common.h"

#include <variant>

enum coopGameType: uint8_t
{
    STANDARD,
    COOPDAILYCHALLENGE,
    BOSSEVENT
};

enum difficulty
{
    EASY,
    MEDIUM,
    HARD
};

enum class gameMode
{
    STANDARD
};

enum mapDivisions
{
    FREEFORALL,
    HORIZONTAL,
    VERTICAL,
    DIAGONAL_LR,
    DIAGONAL_RL,
    DEFAULT,
    RADIOACTIVE,
    STAIRS,
};

enum beginnerMaps
{
    TUTORIAL,
    ALPINERUN,
    CANDYFALLS,
    CARVED,
    CUBISM,
    ENDOFTHEROAD,
    FOURCIRCLES,
    FROZENOVER,
    HEDGE,
    INTHELOOP,
    LOGS,
    LOTUSISLAND,
    ONETWOTREE,
    PARKPATH,
    RESORT,
    SCRAPYARD,
    SKATES,
    THECABIN,
    TOWNCENTRE,
    TREESTUMP,
    WINTERPARK
};

enum intermediateMaps
{
    ADORASTEMPLE = 21,
    BALANCE,
    BAZAAR,
    BLOONARIUSPRIME,
    CHUTES,
    COVEREDGARDEN,
    CRACKED,
    DOWNSTREAM,
    ENCRYPTED,
    FIRINGRANGE,
    HAUNTED,
    KARTSNDARTS,
    MOONLANDING,
    QUARRY,
    QUIETSTREET,
    RAKE,
    SPICEISLANDS,
    SPRINGSPRING,
    STREAMBED
};

enum advancedMaps
{
    ANOTHERBRICK = 40,
    CARGO,
    CORNFIELD,
    GEARED,
    HIGHFINANCE,
    MESA,
    MIDNIGHTMANSION,
    OFFTHECOAST,
    PATSPOND,
    PENINSULA,
    SPILLWAY,
    SUNKENCOLUMNS,
    UNDERGROUND,
    XFACTOR
};

enum expertMaps
{
    BLONS = 53,
    BLOODYPUDDLES,
    DARKCASTLE,
    FLOODEDVALLEY,
    INFERNAL,
    MUDDLEPUDDLES,
    QUAD,
    RAVINE,
    SANCTUARY,
    WORKSHOP,
    OUCH
};

typedef std::variant<beginnerMaps, intermediateMaps, advancedMaps, expertMaps> maps;

string getMapString(maps map);

string getDifficultyString(difficulty dif);

string getGameModeString(gameMode gm);

#endif //BLOONSTD6TOOLS_GAMECONFIG_H
