//
// Created by Rosier on 2023-02-05.
//

#include "GameConfig.h"

string getMapString(maps map)
{
    if (std::holds_alternative<beginnerMaps>(map))
    {
        switch (std::get<beginnerMaps>(map))
        {
            case TUTORIAL:
                return "Tutorial";
            case ALPINERUN:
                return "AlpineRun";
            case CANDYFALLS:
                return "CandyFalls";
            case CARVED:
                return "Carved";
            case CUBISM:
                return "Cubism";
            case ENDOFTHEROAD:
                return "EndOfTheRoad";
            case FOURCIRCLES:
                return "FourCircles";
            case FROZENOVER:
                return "FrozenOver";
            case HEDGE:
                return "Hedge";
            case INTHELOOP:
                return "InTheLoop";
            case LOGS:
                return "Logs";
            case LOTUSISLAND:
                return "LotusIsland";
            case ONETWOTREE:
                return "OneTwoTree";
            case PARKPATH:
                return "ParkPath";
            case RESORT:
                return "Resort";
            case SCRAPYARD:
                return "Scrapyard";
            case SKATES:
                return "Skates";
            case THECABIN:
                return "TheCabin";
            case TOWNCENTRE:
                return "TownCentre";
            case TREESTUMP:
                return "TreeStump";
            case WINTERPARK:
                return "WinterPark";
        }
    }
    else if (std::holds_alternative<intermediateMaps>(map))
    {
        switch (std::get<intermediateMaps>(map))
        {
            case ADORASTEMPLE:
                return "AdorasTemple";
            case BALANCE:
                return "Balance";
            case BAZAAR:
                return "Bazaar";
            case BLOONARIUSPRIME:
                return "BloonariusPrime";
            case CHUTES:
                return "Chutes";
            case COVEREDGARDEN:
                return "CoveredGarden";
            case CRACKED:
                return "Cracked";
            case DOWNSTREAM:
                return "Downstream";
            case ENCRYPTED:
                return "Encrypted";
            case FIRINGRANGE:
                return "FiringRange";
            case HAUNTED:
                return "Haunted";
            case KARTSNDARTS:
                return "KartsNDarts";
            case MOONLANDING:
                return "MoonLanding";
            case QUARRY:
                return "Quarry";
            case QUIETSTREET:
                return "QuietStreet";
            case RAKE:
                return "Rake";
            case SPICEISLANDS:
                return "SpiceIslands";
            case SPRINGSPRING:
                return "SpringSpring";
            case STREAMBED:
                return "Streambed";
        }
    }
    else if (std::holds_alternative<advancedMaps>(map))
    {
        switch (std::get<advancedMaps>(map))
        {
            case ANOTHERBRICK:
                return "AnotherBrick";
            case CARGO:
                return "Cargo";
            case CORNFIELD:
                return "Cornfield";
            case GEARED:
                return "Geared";
            case HIGHFINANCE:
                return "HighFinance";
            case MESA:
                return "Mesa";
            case MIDNIGHTMANSION:
                return "MidnightMansion";
            case OFFTHECOAST:
                return "OffTheCoast";
            case PATSPOND:
                return "PatsPond";
            case PENINSULA:
                return "Peninsula";
            case SPILLWAY:
                return "Spillway";
            case SUNKENCOLUMNS:
                return "SunkenColumns";
            case UNDERGROUND:
                return "Underground";
            case XFACTOR:
                return "XFactor";
        }
    }
    else if (std::holds_alternative<expertMaps>(map))
    {
        switch (std::get<expertMaps>(map))
        {
            case BLONS:
                return "Blons";
            case BLOODYPUDDLES:
                return "BloodyPuddles";
            case DARKCASTLE:
                return "DarkCastle";
            case FLOODEDVALLEY:
                return "FloodedValley";
            case INFERNAL:
                return "Infernal";
            case MUDDLEPUDDLES:
                return "MuddyPuddles";
            case QUAD:
                return "Quad";
            case RAVINE:
                return "Ravine";
            case SANCTUARY:
                return "Sanctuary";
            case WORKSHOP:
                return "Workshop";
            case OUCH:
                return "#ouch";
        }
    }
    return "";
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