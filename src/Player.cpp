//
// Created by Rosier on 2023-01-16.
//

#include "Player.h"

#include <utility>



// Private

// Public
Player::Player(uint8_t id, string globalId) :
               m_id{id}, m_apiId{std::move(globalId)} {}


void Player::updateInfo(json::object &json)
{
    m_apiId = json.at("linkId").as_string();
    m_name = json["displayName"].as_string();
    m_safeName = json["safeName"].as_string();
    m_level = json["rank"].to_number<int>();
    m_chosenHero = json["hero"].as_string();
    for (auto mk : json["requestedMods"].as_array())
    {
        m_enabledMonkeyKnowledge.emplace_back(mk.as_string().c_str());
    }

    // Tower Cosmetics
    for (auto tower : json.at("cosmetics").at("towerCosmetics").as_object())
    {
        // Tower Skins
        if (tower.value().as_object().if_contains("fullSkin"))
        {
            if (tower.value().at("fullSkin") != tower.key())
            {
                m_cosmetics.push_back(
                        std::make_shared<TowerSkin>(tower.key(), tower.value().at("fullSkin").as_string().c_str()));
            }
        }
        // Tower Asset Changes
        for (auto asset : tower.value().at("assetChangeIds").as_array()) {
            m_cosmetics.push_back(std::make_shared<TowerAsset>(tower.key(), asset.as_string().c_str()));
        }
        // Tower Pet
        if (tower.value().as_object().if_contains("pet"))
        {
            m_cosmetics.push_back(std::make_shared<TowerPet>(tower.key(), tower.value().at("pet").as_string().c_str()));
        }
        // Tower Props
        for (auto prop : tower.value().at("props").as_array()) {
            m_cosmetics.push_back(std::make_shared<TowerAsset>(tower.key(), prop.as_string().c_str()));
        }

    }

    if (!json.at("cosmetics").at("towerSetCosmetics").as_object().empty()) {
        for (auto cosmetic: json.at("cosmetics").at("towerSetCosmetics").at("Primary").as_array()) {
            m_cosmetics.push_back(std::make_shared<TowerSetCosmetic>(cosmetic.as_string().c_str()));
        }
    }

    for (auto bloon : json.at("cosmetics").at("bloonAssetChanges").as_object())
    {
        m_cosmetics.push_back(std::make_shared<BloonAsset>(bloon.key(), bloon.value().as_string().c_str()));
    }

    string moabDecal, bloonDecal;
    for (auto bloon : json.at("cosmetics").at("bloonDecals").as_object())
    {
        if (bloon.key() != "Moab" || bloon.key() != "Bfb" || bloon.key() != "Zomg" || bloon.key() != "Ddt" || bloon.key() != "Bad")
        {
            if (bloonDecal.empty())
            {
                bloonDecal = bloon.value().as_string();
            }
        }
        else
        {
            if (moabDecal.empty())
            {
                moabDecal = bloon.value().as_string();
            }
        }
    }

    if (moabDecal == bloonDecal)
    {
        m_cosmetics.push_back(std::make_shared<BloonDecal>(bloonDecal, false));
    }
    else
    {
        m_cosmetics.push_back(std::make_shared<BloonDecal>(bloonDecal, true));
        if (!moabDecal.empty()) {
            m_cosmetics.push_back(std::make_shared<BloonDecal>(moabDecal, false));
        }
    }

    if (json.at("cosmetics").as_object().if_contains("globalBloonPopFX"))
    {
        m_cosmetics.push_back(std::make_shared<PopFx>(json.at("cosmetics").at("globalBloonPopFX").as_string().c_str()));
    }

    for (auto power : json.at("cosmetics").at("powerAssetChanges").as_object())
    {
        m_cosmetics.push_back(std::make_shared<PowerSkin>(power.key(), power.value().as_string().c_str()));
    }

    for (auto name : json.at("namedMonkeyNames").as_object())
    {
        string towerKey{name.key()};
        string towerValue{name.value().as_string()};
        if (towerKey[towerKey.size()-1] == '1')
        {
            towerKey.pop_back();

            for (auto pos{towerValue.find(' ')}; pos != string::npos; pos = towerValue.find(' '))
            {
                towerValue.erase(pos, 1);
            }
        }

        if (towerKey != towerValue)
        {
            m_namedMonkeys.emplace(name.key(), name.value().as_string());
        }
    }

    m_banner = json.at("banner").as_string();

    for (auto hero : json.at("availableHeroes").as_array())
    {
        m_availableHeroes.emplace_back(hero.as_string());
    }

    m_autoplaySetting = json.at("initialAutoplaySetting").as_bool();
}

json::object Player::getInfo()
{
    json::object playerInfo;

    playerInfo["linkId"] = m_apiId;
    playerInfo["displayName"] = m_name;
    playerInfo["safeName"] = m_safeName;
    playerInfo["rank"] = m_level;
    playerInfo["hero"] = m_chosenHero;
    playerInfo["requestedMods"] = json::array(m_enabledMonkeyKnowledge.begin(), m_enabledMonkeyKnowledge.end());
    playerInfo["medalIndex"] = 2;
    playerInfo["medalCount"] = 1;

    playerInfo["cosmetics"].emplace_object();

    playerInfo.at("cosmetics").as_object()["towerCosmetics"].emplace_object();
    playerInfo.at("cosmetics").as_object()["towerSetCosmetics"].emplace_object();
    playerInfo.at("cosmetics").as_object()["globalTowerCosmetics"].emplace_array();
    playerInfo.at("cosmetics").as_object()["bloonAssetChanges"].emplace_object();
    playerInfo.at("cosmetics").as_object()["bloonDecals"].emplace_object();
    playerInfo.at("cosmetics").as_object()["bloonPopFX"].emplace_object();
    playerInfo.at("cosmetics").as_object()["powerAssetChanges"].emplace_object();


    json::object &towerCosmetic = playerInfo.at("cosmetics").at("towerCosmetics").as_object();

    for (auto hero : Heroes)
    {
        json::object &heroObject = towerCosmetic[hero].emplace_object();
        heroObject.emplace("fullSkin", hero);
        heroObject["assetChangeIds"].emplace_array();
        heroObject["props"].emplace_array();
    }

    for (auto &cosmetic : m_cosmetics)
    {
        bool hero{false};
        if (cosmetic->isTowerCosmetic())
        {
            std::shared_ptr<TowerCosmetic> generalTowerCosmetic(std::static_pointer_cast<TowerCosmetic>(cosmetic));
            towerCosmetic[generalTowerCosmetic->m_towerName].emplace_object();
            for (auto heroName : Heroes)
            {
                if (generalTowerCosmetic->m_towerName == heroName)
                {
                    hero = true;
                    towerCosmetic.at(generalTowerCosmetic->m_towerName).as_object()["fullSkin"].emplace_string() = generalTowerCosmetic->m_towerName;
                    break;
                }
            }

            towerCosmetic.at(generalTowerCosmetic->m_towerName).as_object()["assetChangeIds"].emplace_array();
            towerCosmetic.at(generalTowerCosmetic->m_towerName).as_object()["props"].emplace_array();
        }
        if (cosmetic->m_section == Cosmetic::CosmeticSection::TowerCosmeticSkin)
        {
            std::shared_ptr<TowerSkin> towerSkin(std::static_pointer_cast<TowerSkin>(cosmetic));
            towerCosmetic.at(towerSkin->m_towerName).at("fullSkin") = towerSkin->m_cosmeticData;
        }
        else if (cosmetic->m_section == Cosmetic::CosmeticSection::TowerCosmeticAsset)
        {
            std::shared_ptr<TowerAsset> towerAsset(std::static_pointer_cast<TowerAsset>(cosmetic));
            towerCosmetic.at(towerAsset->m_towerName).at("assetChangeIds").as_array().emplace_back(towerAsset->m_cosmeticData);
        }
        else if (cosmetic->m_section == Cosmetic::CosmeticSection::TowerCosmeticProp)
        {
            std::shared_ptr<TowerProp> towerProp(std::static_pointer_cast<TowerProp>(cosmetic));
            towerCosmetic.at(towerProp->m_towerName).at("props").as_array().emplace_back(towerProp->m_cosmeticData);
        }
        else if (cosmetic->m_section == Cosmetic::CosmeticSection::TowerCosmeticPet)
        {
            std::shared_ptr<TowerPet> towerPet(std::static_pointer_cast<TowerPet>(cosmetic));
            towerCosmetic.at(towerPet->m_towerName).as_object()["pet"] = towerPet->m_cosmeticData;
        }
        else if (cosmetic->m_section == Cosmetic::CosmeticSection::TowerSetCosmetic)
        {
            std::shared_ptr<TowerSetCosmetic> towerSet(std::static_pointer_cast<TowerSetCosmetic>(cosmetic));
            playerInfo.at("cosmetics").at("towerSetCosmetics").as_object()["Primary"].emplace_array().emplace_back(towerSet->m_cosmeticData);
            playerInfo.at("cosmetics").at("towerSetCosmetics").as_object()["Military"].emplace_array().emplace_back(towerSet->m_cosmeticData);
            playerInfo.at("cosmetics").at("towerSetCosmetics").as_object()["Magic"].emplace_array().emplace_back(towerSet->m_cosmeticData);
            playerInfo.at("cosmetics").at("towerSetCosmetics").as_object()["Support"].emplace_array().emplace_back(towerSet->m_cosmeticData);
        }
        else if (cosmetic->m_section == Cosmetic::CosmeticSection::BloonAsset)
        {
            std::shared_ptr<BloonAsset> bloonAsset(std::static_pointer_cast<BloonAsset>(cosmetic));
            playerInfo.at("cosmetics").at("bloonAssetChanges").as_object().emplace(bloonAsset->m_bloon, bloonAsset->m_cosmeticData);
        }
        else if (cosmetic->m_section == Cosmetic::CosmeticSection::BloonDecal)
        {
            std::shared_ptr<BloonDecal> bloonDecal(std::static_pointer_cast<BloonDecal>(cosmetic));
            if (playerInfo.at("cosmetics").at("bloonDecals").as_object().empty() || bloonDecal->m_BloonOnly)
            {
                for (auto bloon: bloonModelNames) {
                    playerInfo.at("cosmetics").at("bloonDecals").as_object().emplace(bloon, bloonDecal->m_cosmeticData);
                }
            }
            if (!bloonDecal->m_BloonOnly)
            {
                for (auto moab : moabModelNames)
                {
                    playerInfo.at("cosmetics").at("bloonDecals").as_object().emplace(moab, bloonDecal->m_cosmeticData);
                }
            }
        }
        else if (cosmetic->m_section == Cosmetic::CosmeticSection::GlobalBloonPopFx)
        {
            std::shared_ptr<PopFx> popFx(std::static_pointer_cast<PopFx>(cosmetic));
            playerInfo.at("cosmetics").as_object()["globalBloonPopFX"].emplace_string() = popFx->m_cosmeticData;
        }
        else if (cosmetic->m_section == Cosmetic::CosmeticSection::PowerAsset)
        {
            std::shared_ptr<PowerSkin> powerSkin(std::static_pointer_cast<PowerSkin>(cosmetic));
            playerInfo.at("cosmetics").at("powerAssetChanges").as_object().emplace(powerSkin->m_powerName, powerSkin->m_cosmeticData);
        }
    }

    playerInfo["namedMonkeyNames"].emplace_object();
    if (!m_namedMonkeys.empty())
    {
        for (auto &tower : Towers)
        {
            auto it = m_namedMonkeys.find(tower);
            if (it != m_namedMonkeys.end())
            {
                playerInfo.at("namedMonkeyNames").as_object().emplace(tower + string("1"), it->second);
            }
            else
            {
                playerInfo.at("namedMonkeyNames").as_object().emplace(tower + string("1"), camelToSpace(tower));
            }
        }
        for (auto &hero : Heroes)
        {
            auto it = m_namedMonkeys.find(hero);
            if (it != m_namedMonkeys.end())
            {
                playerInfo.at("namedMonkeyNames").as_object().emplace(camelToSpace(hero), it->second);
            }
            else
            {
                playerInfo.at("namedMonkeyNames").as_object().emplace(camelToSpace(hero), camelToSpace(hero));
            }
        }
        for (auto &heroSkin : HeroSkins)
        {
            auto it = m_namedMonkeys.find(heroSkin);
            if (it != m_namedMonkeys.end())
            {
                playerInfo.at("namedMonkeyNames").as_object().emplace(heroSkin, it->second);
            }
            else
            {
                playerInfo.at("namedMonkeyNames").as_object().emplace(heroSkin, heroSkin);
            }
        }

    }


    playerInfo["banner"] = m_banner;
    playerInfo["availableHeroes"].emplace_array();

    for (auto &hero : m_availableHeroes)
    {
        playerInfo.at("availableHeroes").as_array().emplace_back(hero);
    }

    playerInfo["initialAutoplaySetting"] = m_autoplaySetting;
    
    return playerInfo;
}


















