//
// Created by Rosier on 2023-02-18.
//

#ifndef BLOONSTD6TOOLS_PLAYERDATA_H
#define BLOONSTD6TOOLS_PLAYERDATA_H

#include "common.h"

#include <boost/json.hpp>

#include <vector>
#include <cstdint>
#include <array>
#include <map>
#include <utility>

namespace json = boost::json;

const std::array kAchievements{1,2,3,4,5,6,7,8,9,10,11,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,67,69,70,71,72,73,74,75,76,77,78,80,81,82,85,86,87,88,89,90,91,93,94,95,96,97,98,99,100,103,104,105,106,107,108,109,110,111,112,113,114,115,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,134,135,136,137,139,140,141,142,143,144,145,147};
const std::array kHiddenAchievements{64,65,66,68,83,84,92,101,102,116,133,138};

struct BossBadge {
    float normalBadges;
    float eliteBadges;
};

struct PopStats {
    int64_t bloonsPopped;
    int coopBloonsPopped;

    int camosPopped;
    int leadsPopped;
    int purplesPopped;
    int regrowsPopped;
    int ceramicsPopped;
    int moabsPopped;
    int bfbsPopped;
    int zomgsPopped;
    int ddtsPopped;
    int badsPopped;
    int bossesPopped;

    bool popCountSaved;
    bool coopPopCountSaved;
    bool camoPoppedSaved;
    bool leadPoppedSaved;
    bool purplePoppedSaved;
    bool regrowPoppedSaved;
    bool ceramicPoppedSaved;
    bool moabPoppedSaved;
    bool bfbPoppedSaved;
    bool zomgPoppedSaved;
    bool ddtPoppedSaved;
    bool badPoppedSaved;
    bool bossPoppedSaved;
};

struct NamedMonkeyStats {
    string name;
    string tower;
    int gamesWon;
    int highestRound;
    int timesPlaced;
    PopStats popStats;
    int timesUpgraded;
    int timesSacrificed;
    int abilitiesUsed;
    int cashGenerated;

    // bool highestRoundSaved;
    // bool timesPlacedSaved;
    // bool popStatsSaved;
};

struct PlayerStats {
    string playerId;
    int playerLevel;
    double playerXp;

    string avatar;
    string banner;

    int gameCount;
    int gamesWon;
    int monkeyTeamWins;

    int highestRound;
    int highestRoundChimps;
    int highestRoundDeflation;

    int monkeysPlaced;
    std::map<string, int> heroesPlacedData;
    std::map<string, int> towersPlacedData;
    string mostExperiencedMonkey;
    double mostExperiencedMonkeyXp;
    int instaMonkeyCollection;
    std::map<string, NamedMonkeyStats> namedMonkeys;

    int bloonsLeaked;

    int64_t cashEarned;
    int64_t coopCashGiven;

    int instasUsed;
    int powersUsed;
    int abilitiesUsed;

    std::vector<int16_t> claimedAchievements;
    std::vector<int16_t> claimedHiddenAchievements;

    std::map<string, int> singlePlayerMedals;
    std::map<string, int> coopMedals;
    std::map<string, BossBadge> bossBadges;

    int dailyRewards;
    int challengesCompleted;
    int collectionChestsOpened;

    int odysseysCompleted;
    int odysseyStars;
    int trophiesEarned;
    int necroBloons;
    int transformingTonics;
    int goldenBloonsPopped;


    bool gameCountSaved;
    bool gamesWonSaved;
    bool monkeyTeamWinsSaved;
    bool highestRoundSaved;
    bool highestRoundChimpsSaved;
    bool highestRoundDeflationSaved;
    bool monkeysPlacedSaved;
    bool mostExperiencedMonkeySaved;
    bool mostExperiencedMonkeyXpSaved;
    bool instaMonkeyCollectionSaved;
    bool bloonsLeakedSaved;
    bool cashEarnedSaved;
    bool cashGivenSaved;
    bool instasUsedSaved;
    bool powersUsedSaved;
    bool abilitiesUsedSaved;
    bool achievementsSaved;
    bool hiddenAchievementsSaved;
    bool dailyRewardSaved;
    bool challengesCompletedSaved;
    bool collectionChestsSaved;
    bool odysseysCompletedSaved;
    bool trophiesEarnedSaved;
    bool necroBloonsSaved;
    bool transformingTonicsSaved;
    bool goldenBloonsPoppedSaved;

    PopStats popStats;

    bool filled{false};

    friend void tag_invoke(const json::value_from_tag&, json::value& jv, PlayerStats const& p);
    friend PlayerStats tag_invoke(json::value_to_tag<PlayerStats>, json::value const& jv);
};


struct PlayerData {
    string playerId;
    string displayName;
    string shortCode;
    string safeName;

    int clan{4};
    string country{"N/A"};
    string continent{"N/A"};
    int avatar{0};

    bool online{false};
    int onlineApp{-1};

    std::vector<string> providers;

    int access{0};
    int age{0};

    bool filled{false};

    PlayerStats stats;


    friend void tag_invoke(const json::value_from_tag&, json::value& jv, PlayerData const& p);
    friend PlayerData tag_invoke(json::value_to_tag<PlayerData>, json::value const& jv);
};

#endif //BLOONSTD6TOOLS_PLAYERDATA_H
