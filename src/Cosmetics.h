//
// Created by Rosier on 2023-01-17.
//

#ifndef BLOONSTD6TOOLS_COSMETICS_H
#define BLOONSTD6TOOLS_COSMETICS_H

#include <map>
#include <utility>

#include "common.h"


struct TrophyStoreItem
{
    enum Category{
        Heroes,
        Monkeys,
        Bloons,
        Coop,
        GameAndUi
    } m_category;

    enum SubCategory{
        PlaceFx,
        Pets,
        UpgradeFx,
        Projectiles,
        Flags,
        PopFx,
        AllBloonSkins,
        BloonSkins,
        MoabSkins,
        StandardEmotes,
        TextEmotes,
        SoundEmotes,
        FullscreenEmotes,
        MusicTracks,
        Avatars,
        PowerSkins,
        ProfileBanners,
        Misc
    } m_subCategory;

    string m_name;
};

struct Cosmetic {
    enum class CosmeticSection {
        TowerCosmeticSkin,
        TowerCosmeticAsset,
        TowerCosmeticPet,
        TowerCosmeticProp,
        TowerSetCosmetic,
        GlobalTowerCosmetic,
        BloonAsset,
        BloonDecal,
        GlobalBloonPopFx,
        PowerAsset
    } m_section{};

    string m_cosmeticData;

    bool isTowerCosmetic();

    // explicit Cosmetic(CosmeticSection section) : m_section(section) {};
};

struct TowerCosmetic : Cosmetic {
    string m_towerName;

    TowerCosmetic(CosmeticSection section, string tower, string data) :
                  Cosmetic{section, std::move(data)},
                  m_towerName{std::move(tower)} {};
};

struct TowerSkin : TowerCosmetic {
    TowerSkin(string tower, string skin) :
              TowerCosmetic{CosmeticSection::TowerCosmeticSkin, std::move(tower), std::move(skin)} {};
};

struct TowerAsset : TowerCosmetic {
    TowerAsset(string tower, string asset) :
               TowerCosmetic{CosmeticSection::TowerCosmeticAsset, std::move(tower), std::move(asset)} {};
};

struct TowerPet : TowerCosmetic {
    TowerPet(string tower, string pet) :
             TowerCosmetic{CosmeticSection::TowerCosmeticPet, std::move(tower), std::move(pet)} {};
};

struct TowerProp : TowerCosmetic {
TowerProp(string tower, string prop) :
          TowerCosmetic{CosmeticSection::TowerCosmeticProp, std::move(tower), std::move(prop)} {};
};

struct TowerSetCosmetic : Cosmetic {
    explicit TowerSetCosmetic(string cosmeticName) :
            Cosmetic{CosmeticSection::TowerSetCosmetic, std::move(cosmeticName)}
    {};
};

struct BloonAsset : Cosmetic {
    string m_bloon;

    BloonAsset(string bloon, string asset) :
               Cosmetic{CosmeticSection::BloonAsset, std::move(asset)},
               m_bloon{std::move(bloon)} {};
};

struct BloonDecal : Cosmetic {
    bool m_BloonOnly;

    explicit BloonDecal(string skin, bool bloonOnly = false) :
            Cosmetic{CosmeticSection::BloonDecal, std::move(skin)},
            m_BloonOnly{bloonOnly}
    {};
};

struct PopFx : Cosmetic {
    explicit PopFx(string popEffect) :
            Cosmetic{CosmeticSection::GlobalBloonPopFx, std::move(popEffect)}
    {};
};

struct PowerSkin : Cosmetic {
    string m_powerName;

    PowerSkin(string power, string asset) :
            Cosmetic{CosmeticSection::PowerAsset, std::move(asset)},
            m_powerName{std::move(power)}
    {};
};


#endif //BLOONSTD6TOOLS_COSMETICS_H
