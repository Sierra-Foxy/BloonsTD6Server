//
// Created by Rosier on 2023-02-05.
//

#ifndef BLOONSTD6TOOLS_GAMECONFIG_H
#define BLOONSTD6TOOLS_GAMECONFIG_H

#include "common.h"

#include <variant>
#include <map>
#include <unordered_map>

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
    TUTORIAL,
    WINTERPARK,
    LAST_BEGINNER = WINTERPARK
};

enum intermediateMaps
{
    FIRST_INTERMEDIATE = 21,
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
    STREAMBED,
    LAST_INTERMEDIATE = STREAMBED
};

enum advancedMaps
{
    FIRST_ADVANCED = 40,
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
    XFACTOR,
    LAST_ADVANCED = XFACTOR
};

enum expertMaps
{
    FIRST_EXPERT = 54,
    BLONS = 54,
    BLOODYPUDDLES,
    DARKCASTLE,
    FLOODEDVALLEY,
    INFERNAL,
    MUDDYPUDDLES,
    QUAD,
    RAVINE,
    SANCTUARY,
    WORKSHOP,
    OUCH,
    LAST_EXPERT = OUCH,
    LAST_MAP = LAST_EXPERT
};

// const int beginnerMaps{20};
// const int intermediateMaps{20};
// const int advancedMaps{20};
// const int expertMaps{20};

// enum maps
// {
//     ALPINERUN,
//     CANDYFALLS,
//     CARVED,
//     CUBISM,
//     ENDOFTHEROAD,
//     FOURCIRCLES,
//     FROZENOVER,
//     HEDGE,
//     INTHELOOP,
//     LOGS,
//     LOTUSISLAND,
//     ONETWOTREE,
//     PARKPATH,
//     RESORT,
//     SCRAPYARD,
//     SKATES,
//     THECABIN,
//     TOWNCENTRE,
//     TREESTUMP,
//     TUTORIAL,
//     WINTERPARK,
//     ADORASTEMPLE,
//     BALANCE,
//     BAZAAR,
//     BLOONARIUSPRIME,
//     CHUTES,
//     COVEREDGARDEN,
//     CRACKED,
//     DOWNSTREAM,
//     ENCRYPTED,
//     FIRINGRANGE,
//     HAUNTED,
//     KARTSNDARTS,
//     MOONLANDING,
//     QUARRY,
//     QUIETSTREET,
//     RAKE,
//     SPICEISLANDS,
//     SPRINGSPRING,
//     STREAMBED,
//     ANOTHERBRICK,
//     CARGO,
//     CORNFIELD,
//     GEARED,
//     HIGHFINANCE,
//     MESA,
//     MIDNIGHTMANSION,
//     OFFTHECOAST,
//     PATSPOND,
//     PENINSULA,
//     SPILLWAY,
//     SUNKENCOLUMNS,
//     UNDERGROUND,
//     XFACTOR,
//     BLONS,
//     BLOODYPUDDLES,
//     DARKCASTLE,
//     FLOODEDVALLEY,
//     INFERNAL,
//     MUDDYPUDDLES,
//     QUAD,
//     RAVINE,
//     SANCTUARY,
//     WORKSHOP,
//     OUCH
// };

static std::unordered_map<int, string> const toMapStringTable {
        {ALPINERUN, "AlpineRun"},
        {CANDYFALLS, "CandyFalls"},
        {CARVED, "Carved"},
        {CUBISM, "Cubism"},
        {ENDOFTHEROAD, "EndOfTheRoad"},
        {FOURCIRCLES, "FourCircles"},
        {FROZENOVER, "FrozenOver"},
        {HEDGE, "Hedge"},
        {INTHELOOP, "InTheLoop"},
        {LOGS, "Logs"},
        {LOTUSISLAND, "LotusIsland"},
        {ONETWOTREE, "OneTwoTree"},
        {PARKPATH, "ParkPath"},
        {RESORT, "Resort"},
        {SCRAPYARD, "Scrapyard"},
        {SKATES, "Skates"},
        {THECABIN, "TheCabin"},
        {TOWNCENTRE, "TownCentre"},
        {TREESTUMP, "TreeStump"},
        {TUTORIAL, "Tutorial"},
        {WINTERPARK, "WinterPark"},
        {ADORASTEMPLE, "AdorasTemple"},
        {BALANCE, "Balance"},
        {BAZAAR, "Bazaar"},
        {BLOONARIUSPRIME, "BloonariusPrime"},
        {CHUTES, "Chutes"},
        {COVEREDGARDEN, "CoveredGarden"},
        {CRACKED, "Cracked"},
        {DOWNSTREAM, "Downstream"},
        {ENCRYPTED, "Encrypted"},
        {FIRINGRANGE, "FiringRange"},
        {HAUNTED, "Haunted"},
        {KARTSNDARTS, "KartsNDarts"},
        {MOONLANDING, "MoonLanding"},
        {QUARRY, "Quarry"},
        {QUIETSTREET, "QuietStreet"},
        {RAKE, "Rake"},
        {SPICEISLANDS, "SpiceIslands"},
        {SPRINGSPRING, "SpringSpring"},
        {STREAMBED, "Streambed"},
        {ANOTHERBRICK, "AnotherBrick"},
        {CARGO, "Cargo"},
        {CORNFIELD, "Cornfield"},
        {GEARED, "Geared"},
        {HIGHFINANCE, "HighFinance"},
        {MESA, "Mesa"},
        {MIDNIGHTMANSION, "MidnightMansion"},
        {OFFTHECOAST, "OffTheCoast"},
        {PATSPOND, "PatsPond"},
        {PENINSULA, "Peninsula"},
        {SPILLWAY, "Spillway"},
        {SUNKENCOLUMNS, "SunkenColumns"},
        {UNDERGROUND, "Underground"},
        {XFACTOR, "XFactor"},
        {BLONS, "Blons"},
        {BLOODYPUDDLES, "BloodyPuddles"},
        {DARKCASTLE, "DarkCastle"},
        {FLOODEDVALLEY, "FloodedValley"},
        {INFERNAL, "Infernal"},
        {MUDDYPUDDLES, "MuddyPuddles"},
        {QUAD, "Quad"},
        {RAVINE, "Ravine"},
        {SANCTUARY, "Sanctuary"},
        {WORKSHOP, "Workshop"},
        {OUCH, "#ouch"}
};

static std::unordered_map<string, int> const toMapEnumTable {
        {"alpinerun", ALPINERUN},
        {"candyfalls", CANDYFALLS},
        {"carved", CARVED},
        {"cubism", CUBISM},
        {"endoftheroad", ENDOFTHEROAD},
        {"fourcircles", FOURCIRCLES},
        {"frozenover", FROZENOVER},
        {"hedge", HEDGE},
        {"intheloop", INTHELOOP},
        {"logs", LOGS},
        {"lotusisland", LOTUSISLAND},
        {"onetwotree", ONETWOTREE},
        {"parkpath", PARKPATH},
        {"resort", RESORT},
        {"scrapyard", SCRAPYARD},
        {"skates", SKATES},
        {"thecabin", THECABIN},
        {"towncentre", TOWNCENTRE},
        {"treestump", TREESTUMP},
        {"tutorial", TUTORIAL},
        {"winterpark", WINTERPARK},
        {"adorastemple", ADORASTEMPLE},
        {"balance", BALANCE},
        {"bazaar", BAZAAR},
        {"bloonariusprime", BLOONARIUSPRIME},
        {"chutes", CHUTES},
        {"coveredgarden", COVEREDGARDEN},
        {"cracked", CRACKED},
        {"downstream", DOWNSTREAM},
        {"encrypted", ENCRYPTED},
        {"firingrange", FIRINGRANGE},
        {"haunted", HAUNTED},
        {"kartsndarts", KARTSNDARTS},
        {"moonlanding", MOONLANDING},
        {"quarry", QUARRY},
        {"quietstreet", QUIETSTREET},
        {"rake", RAKE},
        {"spiceislands", SPICEISLANDS},
        {"springspring", SPRINGSPRING},
        {"streambed", STREAMBED},
        {"anotherbrick", ANOTHERBRICK},
        {"cargo", CARGO},
        {"cornfield", CORNFIELD},
        {"geared", GEARED},
        {"highfinance", HIGHFINANCE},
        {"mesa", MESA},
        {"midnightmansion", MIDNIGHTMANSION},
        {"offthecoast", OFFTHECOAST},
        {"patspond", PATSPOND},
        {"peninsula", PENINSULA},
        {"spillway", SPILLWAY},
        {"sunkencolumns", SUNKENCOLUMNS},
        {"underground", UNDERGROUND},
        {"xfactor", XFACTOR},
        {"blons", BLONS},
        {"bloodypuddles", BLOODYPUDDLES},
        {"darkcastle", DARKCASTLE},
        {"floodedvalley", FLOODEDVALLEY},
        {"infernal", INFERNAL},
        {"muddypuddles", MUDDYPUDDLES},
        {"quad", QUAD},
        {"ravine", RAVINE},
        {"sanctuary", SANCTUARY},
        {"workshop", WORKSHOP},
        {"#ouch", OUCH}
};

int getMapEnum(const string& str);

string getMapString(int map);

string getDifficultyString(difficulty dif);

string getGameModeString(gameMode gm);

#endif //BLOONSTD6TOOLS_GAMECONFIG_H
