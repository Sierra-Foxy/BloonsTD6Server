//
// Created by Rosier on 2023-01-16.
//

#ifndef BLOONSTD6TOOLS_TOWERS_H
#define BLOONSTD6TOOLS_TOWERS_H

#include <array>
#include <vector>

#include "common.h"

string camelToSpace(const string &str);


struct Tower{
    string name;
    bool isHero;

    std::vector<string> skins;
    std::vector<string> pets;
};

const std::array Heroes{
    "AdmiralBrickell",
    "Adora",
    "Benjamin",
    "CaptainChurchill",
    "Etienne",
    "Ezili",
    "Geraldo",
    "Gwendolin",
    "ObynGreenfoot",
    "PatFusty",
    "Psi",
    "Quincy",
    "Sauda",
    "StrikerJones"
};

const std::array HeroSkins{
    "Wolfpack Quincy Skin",
    "Cyber Quincy Skin",
    "Harlegwen Skin",
    "Scientist Gwendolin Skin",
    "Biker Bones Skin",
    "Octojones Skin",
    "Mountain Guardian Skin",
    "Ocean Guardian Skin",
    "Sentai Churchill Skin",
    "Sleigh Churchill Skin",
    "Benjammin DJ Skin",
    "Sushi Bento Skin",
    "Galaxili Skin",
    "Smudge Catt Skin",
    "Fusty the Snowman Skin",
    "Joan of Arc Adora Skin",
    "Voidora Skin",
    "Dread Pirate Brickell Skin",
    "ETn Skin",
    "Viking Sauda Skin",
    "Psimbals Skin"
};

const std::array Towers{
    "Alchemist",
    "BananaFarm",
    "BombShooter",
    "BoomerangMonkey",
    "DartlingGunner",
    "DartMonkey",
    "Druid",
    "EngineerMonkey",
    "GlueGunner",
    "HeliPilot",
    "IceMonkey",
    "MonkeyAce",
    "MonkeyBuccaneer",
    "MonkeySub",
    "MonkeyVillage",
    "MortarMonkey",
    "NinjaMonkey",
    "SniperMonkey",
    "SpikeFactory",
    "StrikerJones",
    "SuperMonkey",
    "TackShooter",
    "WizardMonkey"
};


#endif //BLOONSTD6TOOLS_TOWERS_H
