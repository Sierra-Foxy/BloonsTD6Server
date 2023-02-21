//
// Created by Rosier on 2023-02-18.
//

#include "PlayerData.h"

#include <stdexcept>

// Private

// Public




// Boss Badge
void tag_invoke(const json::value_from_tag&, json::value& jv, BossBadge const& b)
{
    jv = {
            {"normalBadges", b.normalBadges},
            {"eliteBadges", b.eliteBadges}
    };
}

BossBadge tag_invoke(const json::value_to_tag<BossBadge>&, json::value const& jv)
{
    return BossBadge{
            jv.at("normalBadges").to_number<float>(),
            jv.at("eliteBadges").to_number<float>()
    };
}

// Pop Stats
void tag_invoke(const json::value_from_tag&, json::value& jv, PopStats const& p)
{
    jv = {
            {"bloonsPopped", p.bloonsPopped},
            {"camosPopped", p.camosPopped},
            {"leadsPopped", p.leadsPopped},
            {"purplesPopped", p.purplesPopped},
            {"regrowsPopped", p.regrowsPopped},
            {"ceramicsPopped", p.ceramicsPopped},
            {"moabsPopped", p.moabsPopped},
            {"bfbsPopped", p.bfbsPopped},
            {"zomgsPopped", p.zomgsPopped},
            {"ddtsPopped", p.ddtsPopped},
            {"badsPopped", p.badsPopped},
            {"bossesPopped", p.bossesPopped},
            {"coopBloonsPopped", p.coopBloonsPopped}
    };
}

PopStats tag_invoke(const json::value_to_tag<PopStats>&, json::value const& jv)
{
    return PopStats{
            jv.at("bloonsPopped").to_number<int64_t>(),
            jv.at("coopBloonsPopped").to_number<int>(),
            jv.at("camosPopped").to_number<int>(),
            jv.at("leadsPopped").to_number<int>(),
            jv.at("purplesPopped").to_number<int>(),
            jv.at("regrowsPopped").to_number<int>(),
            jv.at("ceramicsPopped").to_number<int>(),
            jv.at("moabsPopped").to_number<int>(),
            jv.at("bfbsPopped").to_number<int>(),
            jv.at("zomgsPopped").to_number<int>(),
            jv.at("ddtsPopped").to_number<int>(),
            jv.at("badsPopped").to_number<int>(),
            jv.at("bossesPopped").to_number<int>()
    };
}

// Named Monkey Stats
void tag_invoke(const json::value_from_tag&, json::value& jv, NamedMonkeyStats const& m)
{
    jv = {
            {"name", m.name},
            {"BaseTower", m.tower},
            {"gamesWon", m.gamesWon},
            {"highestRound", m.highestRound},
            {"timesPlaced", m.timesPlaced},
            {"totalPopCount", m.popStats.bloonsPopped},
            {"totalCoopPopCount", m.popStats.coopBloonsPopped},
            {"camoBloonsPopped", m.popStats.camosPopped},
            {"leadBloonsPopped", m.popStats.leadsPopped},
            {"purpleBloonsPopped", m.popStats.purplesPopped},
            {"regrowBloonsPopped", m.popStats.regrowsPopped},
            {"ceramicBloonsPopped", m.popStats.ceramicsPopped},
            {"moabsPopped", m.popStats.moabsPopped},
            {"bfbsPopped", m.popStats.bfbsPopped},
            {"zomgsPopped", m.popStats.zomgsPopped},
            {"ddtsPopped", m.popStats.ddtsPopped},
            {"badsPopped", m.popStats.badsPopped},
            {"bossesPopped", m.popStats.bossesPopped},
            {"timesUpgraded", m.timesUpgraded},
            {"timesSacrificed", m.timesSacrificed},
            {"cashGenerated", m.cashGenerated},
            {"abilitiesUsed", m.abilitiesUsed}
    };
}

NamedMonkeyStats tag_invoke(const json::value_to_tag<NamedMonkeyStats>&, json::value const& jv)
{
    NamedMonkeyStats s{jv.at("name").as_string().c_str(), jv.at("BaseTower").as_string().c_str()};
    try {
        s.gamesWon = jv.at("gamesWon").to_number<int>();
    } catch (std::out_of_range&) {}
    try {
        s.highestRound = jv.at("highestRound").to_number<int>();
    } catch (std::out_of_range&) {}
    try {
        s.timesPlaced = jv.at("timesPlaced").to_number<int>();
    } catch (std::out_of_range&) {}
    try {
        s.timesUpgraded = jv.at("timesUpgraded").to_number<int>();
    } catch (std::out_of_range&) {}
    try {
        s.timesSacrificed = jv.at("timesSacrificed").to_number<int>();
    } catch (std::out_of_range&) {}
    try {
        s.cashGenerated = jv.at("cashGenerated").to_number<int>();
    } catch (std::out_of_range&) {}
    try {
        s.abilitiesUsed = jv.at("abilitiesUsed").to_number<int>();
    } catch (std::out_of_range&) {}
    try {
        s.popStats.bloonsPopped = jv.at("totalPopCount").to_number<int>();
    } catch (std::out_of_range&) {}
    try {
        s.popStats.camosPopped = jv.at("camoBloonsPopped").to_number<int>();
    } catch (std::out_of_range&) {}
    try {
        s.popStats.leadsPopped = jv.at("leadBloonsPopped").to_number<int>();
    } catch (std::out_of_range&) {}
    try {
        s.popStats.purplesPopped = jv.at("purpleBloonsPopped").to_number<int>();
    } catch (std::out_of_range&) {}
    try {
        s.popStats.regrowsPopped = jv.at("regrowBloonsPopped").to_number<int>();
    } catch (std::out_of_range&) {}
    try {
        s.popStats.ceramicsPopped = jv.at("ceramicBloonsPopped").to_number<int>();
    } catch (std::out_of_range&) {}
    try {
        s.popStats.moabsPopped = jv.at("moabsPopped").to_number<int>();
    } catch (std::out_of_range&) {}
    try {
        s.popStats.bfbsPopped = jv.at("bfbsPopped").to_number<int>();
    } catch (std::out_of_range&) {}
    try {
        s.popStats.zomgsPopped = jv.at("zomgsPopped").to_number<int>();
    } catch (std::out_of_range&) {}
    try {
        s.popStats.ddtsPopped = jv.at("ddtsPopped").to_number<int>();
    } catch (std::out_of_range&) {}
    try {
        s.popStats.badsPopped = jv.at("badsPopped").to_number<int>();
    } catch (std::out_of_range&) {}
    return s;
}


// Player Stats
void tag_invoke(const json::value_from_tag&, json::value& jv, PlayerStats const& p)
{
    jv = {
        {"playerId", p.playerId},
        {"playerRank", p.playerLevel},
        {"playerXp", p.playerXp},
        {"avatar", p.avatar},
        {"banner", p.banner},
        {"gameCount", p.gameCount},
        {"gamesWon", p.gamesWon},
        {"highestRound", p.highestRound},
        {"monkeysPlaced", p.monkeysPlaced},
        {"bloonsLeaked", p.bloonsLeaked},
        {"cashEarned", p.cashEarned},
        {"instaMonkeysUsed", p.instasUsed},
        {"powersUsed", p.powersUsed},
        {"abilitiesUsed", p.abilitiesUsed},
        {"coopCashGiven", p.coopCashGiven},
        {"achievementsClaimed", json::value_from(p.claimedAchievements)},
        {"hiddenAchievementsClaimed", json::value_from(p.claimedHiddenAchievements)},
        {"achievements", json::value_from(kAchievements)},
        {"hiddenAchievements", json::value_from(kHiddenAchievements)},
        {"heroesPlacedData", json::value_from(p.heroesPlacedData)},
        {"towersPlacedData", json::value_from(p.towersPlacedData)},
        {"spMedals", json::value_from(p.singlePlayerMedals)},
        {"coopMedals", json::value_from(p.coopMedals)},
        {"raceMedals", {}},
        {"bossMedals", {}},
        {"ctLocalMedals", {}},
        {"ctGlobalMedals", {}},
        {"bossBadges", json::value_from(p.bossBadges)},
        {"dailyRewards", p.dailyRewards},
        {"challengesCompleted", p.challengesCompleted},
        {"savedStats", {
                {"PlayerStatsGamesPlayed", p.gameCountSaved},
                {"PlayerStatsGamesWon", p.gamesWonSaved},
                {"PlayerStatsHighestRound", p.highestRoundSaved},
                {"PlayerStatsMonkeysPlaced", p.monkeysPlacedSaved},
                {"PlayerStatsPopCount", p.popStats.popCountSaved},
                {"PlayerStatsCoopPopCount", p.popStats.coopPopCountSaved},
                {"PlayerStatsCamoPopped", p.popStats.camoPoppedSaved},
                {"PlayerStatsLeadPopped", p.popStats.leadPoppedSaved},
                {"PlayerStatsPurplePopped", p.popStats.purplePoppedSaved},
                {"PlayerStatsRegrowPopped", p.popStats.regrowPoppedSaved},
                {"PlayerStatsCeramicPopped", p.popStats.ceramicPoppedSaved},
                {"PlayerStatsMoabsPopped", p.popStats.moabPoppedSaved},
                {"PlayerStatsBfbsPopped", p.popStats.bfbPoppedSaved},
                {"PlayerStatsZomgsPopped", p.popStats.zomgPoppedSaved},
                {"PlayerStatsDdtsPopped", p.popStats.ddtPoppedSaved},
                {"PlayerStatsBadsPopped", p.popStats.badPoppedSaved},
                {"PlayerStatsBossesPopped", p.popStats.bossPoppedSaved},
                {"PlayerStatsBloonsLeaked", p.bloonsLeakedSaved},
                {"PlayerStatsCashGenerated", p.cashEarnedSaved},
                {"PlayerStatsCashGifted", p.cashGivenSaved},
                {"PlayerStatsAbilitiesUsed", p.abilitiesUsedSaved},
                {"PlayerStatsPowersUsed", p.powersUsedSaved},
                {"PlayerStatsInstaMonkeysUsed", p.instasUsedSaved},
                {"PlayerStatsDailysOpened", p.dailyRewardSaved},
                {"PlayerStatsChallengesCompleted", p.challengesCompletedSaved},
                {"Achievements", p.achievementsSaved},
                {"HiddenAchievements", p.hiddenAchievementsSaved},
                {"PlayerStatsOdysseysCompleted", p.odysseysCompletedSaved},
                {"PlayerStatsLifetimeTrophies", p.trophiesEarnedSaved},
                {"PlayerStatsNecroBloons", p.necroBloonsSaved},
                {"PlayerStatsTransformingTonics", p.transformingTonicsSaved},
                {"Most Experienced Monkey", p.mostExperiencedMonkeySaved},
                {"Most Experienced Monkey Xp", p.mostExperiencedMonkeyXpSaved},
                {"Insta Monkey Collection", p.instaMonkeyCollectionSaved},
                {"Collection Chests Opened", p.collectionChestsSaved},
                {"GoldenBloonsPopped", p.goldenBloonsPoppedSaved},
                {"MonkeyTeamWins", p.monkeyTeamWinsSaved},
                {"PlayerStatsHighestRoundCHIMPS", p.highestRoundChimpsSaved},
                {"PlayerStatsHighestRoundDeflation", p.highestRoundDeflationSaved},
                {"PlayerStatsDamageDoneToBosses", false}
        }
        },
        {"namedMonkeyStats", json::value_from(p.namedMonkeys)},
        {"savedNamedMonkeyStats", {}},
        {"totalOdysseysCompleted", p.odysseysCompleted},
        {"totalOdysseyStars", p.odysseyStars},
        {"totalTrophiesEarned", p.trophiesEarned},
        {"statsVersion", 1},
        {"necroBloonsReanimated", p.necroBloons},
        {"transformingTonicsUsed", p.transformingTonics},
        {"mostExperiencedMonkey", p.mostExperiencedMonkey},
        {"mostExperiencedMonkeyXp", p.mostExperiencedMonkeyXp},
        {"instaMonkeyCollection", p.instaMonkeyCollection},
        {"collectionChestsOpened", p.collectionChestsOpened},
        {"goldenBloonsPopped", p.goldenBloonsPopped},
        {"monkeyTeamsWins", p.monkeyTeamWins},
        {"paragonsPurchasedData", {}},
        {"highestRoundCHIMPS", p.highestRoundChimps},
        {"highestRoundDeflation", p.highestRoundDeflation}
    };
    // jv.as_object().insert(json::value_from(p.popStats));
    json::object ps = json::value_from(p.popStats).as_object();
    for (auto i{ps.begin()}; i != ps.end(); i++)
    {
        jv.as_object().insert(std::make_pair(i->key(), i->value()));
    }
    for (const auto& monkey : p.namedMonkeys)
    {
        jv.at("savedNamedMonkeyStats").as_object()[monkey.first] = {
                {"PlayerStatsHighestRound", true},
                {"PlayerStatsTimesPlaced", true},
                {"PlayerStatsPopCount", true}
        };
    }
}

PlayerStats tag_invoke(json::value_to_tag<PlayerStats>, json::value const& jv)
{
    PlayerStats p{
            jv.at("playerId").as_string().c_str(),
            jv.at("playerRank").to_number<int>(),
            jv.at("playerXp").to_number<double>(),
            jv.at("avatar").as_string().c_str(),
            jv.at("banner").as_string().c_str(),
            jv.at("gameCount").to_number<int>(),
            jv.at("gamesWon").to_number<int>(),
            jv.at("monkeyTeamsWins").to_number<int>(),
            jv.at("highestRound").to_number<int>(),
            jv.at("highestRoundCHIMPS").to_number<int>(),
            jv.at("highestRoundDeflation").to_number<int>(),
            jv.at("monkeysPlaced").to_number<int>(),
            json::value_to<std::map<string, int>>(jv.at("heroesPlacedData")),
            json::value_to<std::map<string, int>>(jv.at("towersPlacedData")),
            jv.at("mostExperiencedMonkey").as_string().c_str(),
            jv.at("mostExperiencedMonkeyXp").to_number<double>(),
            jv.at("instaMonkeyCollection").to_number<int>(),
            json::value_to<std::map<string, NamedMonkeyStats>>(jv.at("namedMonkeyStats")),
            jv.at("bloonsLeaked").to_number<int>(),
            jv.at("cashEarned").to_number<int64_t>(),
            jv.at("coopCashGiven").to_number<int64_t>(),
            jv.at("instaMonkeysUsed").to_number<int>(),
            jv.at("powersUsed").to_number<int>(),
            jv.at("abilitiesUsed").to_number<int>(),
            json::value_to<std::vector<int16_t>>(jv.at("achievementsClaimed")),
            json::value_to<std::vector<int16_t>>(jv.at("hiddenAchievementsClaimed")),
            json::value_to<std::map<string, int>>(jv.at("spMedals")),
            json::value_to<std::map<string, int>>(jv.at("coopMedals")),
            json::value_to<std::map<string, BossBadge>>(jv.at("bossBadges")),
            jv.at("dailyRewards").to_number<int>(),
            jv.at("challengesCompleted").to_number<int>(),
            jv.at("collectionChestsOpened").to_number<int>(),
            jv.at("totalOdysseysCompleted").to_number<int>(),
            jv.at("totalOdysseyStars").to_number<int>(),
            jv.at("totalTrophiesEarned").to_number<int>(),
            jv.at("necroBloonsReanimated").to_number<int>(),
            jv.at("transformingTonicsUsed").to_number<int>(),
            jv.at("goldenBloonsPopped").to_number<int>(),
            jv.at("savedStats").at("PlayerStatsGamesPlayed").as_bool(),
            jv.at("savedStats").at("PlayerStatsGamesWon").as_bool(),
            jv.at("savedStats").at("MonkeyTeamWins").as_bool(),
            jv.at("savedStats").at("PlayerStatsHighestRound").as_bool(),
            jv.at("savedStats").at("PlayerStatsHighestRoundCHIMPS").as_bool(),
            jv.at("savedStats").at("PlayerStatsHighestRoundDeflation").as_bool(),
            jv.at("savedStats").at("PlayerStatsMonkeysPlaced").as_bool(),
            jv.at("savedStats").at("Most Experienced Monkey").as_bool(),
            jv.at("savedStats").at("Most Experienced Monkey Xp").as_bool(),
            jv.at("savedStats").at("Insta Monkey Collection").as_bool(),
            jv.at("savedStats").at("PlayerStatsBloonsLeaked").as_bool(),
            jv.at("savedStats").at("PlayerStatsCashGenerated").as_bool(),
            jv.at("savedStats").at("PlayerStatsCashGifted").as_bool(),
            jv.at("savedStats").at("PlayerStatsInstaMonkeysUsed").as_bool(),
            jv.at("savedStats").at("PlayerStatsPowersUsed").as_bool(),
            jv.at("savedStats").at("PlayerStatsAbilitiesUsed").as_bool(),
            jv.at("savedStats").at("Achievements").as_bool(),
            jv.at("savedStats").at("HiddenAchievements").as_bool(),
            jv.at("savedStats").at("PlayerStatsDailysOpened").as_bool(),
            jv.at("savedStats").at("PlayerStatsChallengesCompleted").as_bool(),
            jv.at("savedStats").at("Collection Chests Opened").as_bool(),
            jv.at("savedStats").at("PlayerStatsOdysseysCompleted").as_bool(),
            jv.at("savedStats").at("PlayerStatsLifetimeTrophies").as_bool(),
            jv.at("savedStats").at("PlayerStatsNecroBloons").as_bool(),
            jv.at("savedStats").at("PlayerStatsTransformingTonics").as_bool(),
            jv.at("savedStats").at("GoldenBloonsPopped").as_bool(),
            json::value_to<PopStats>(jv),
            true
    };
    p.popStats.popCountSaved = jv.at("savedStats").at("PlayerStatsPopCount").as_bool();
    p.popStats.coopPopCountSaved = jv.at("savedStats").at("PlayerStatsCoopPopCount").as_bool();
    p.popStats.camoPoppedSaved = jv.at("savedStats").at("PlayerStatsCamoPopped").as_bool();
    p.popStats.leadPoppedSaved = jv.at("savedStats").at("PlayerStatsLeadPopped").as_bool();
    p.popStats.purplePoppedSaved = jv.at("savedStats").at("PlayerStatsPurplePopped").as_bool();
    p.popStats.regrowPoppedSaved = jv.at("savedStats").at("PlayerStatsRegrowPopped").as_bool();
    p.popStats.ceramicPoppedSaved = jv.at("savedStats").at("PlayerStatsCeramicPopped").as_bool();
    p.popStats.moabPoppedSaved = jv.at("savedStats").at("PlayerStatsMoabsPopped").as_bool();
    p.popStats.bfbPoppedSaved = jv.at("savedStats").at("PlayerStatsBfbsPopped").as_bool();
    p.popStats.zomgPoppedSaved = jv.at("savedStats").at("PlayerStatsZomgsPopped").as_bool();
    p.popStats.ddtPoppedSaved = jv.at("savedStats").at("PlayerStatsDdtsPopped").as_bool();
    p.popStats.badPoppedSaved = jv.at("savedStats").at("PlayerStatsBadsPopped").as_bool();
    p.popStats.bossPoppedSaved = jv.at("savedStats").at("PlayerStatsBossesPopped").as_bool();
    return p;
}


// Player Data
void tag_invoke(const json::value_from_tag&, json::value& jv, PlayerData const& p)
{
    jv = {
            {"nkapiID", p.playerId},
            {"displayName", p.displayName},
            {"clan", p.clan},
            {"country", p.country},
            {"continent", p.continent},
            {"avatar", p.avatar},
            {"online", p.online},
            {"onlineApp", p.onlineApp},
            {"providersAvailable", json::value_from(p.providers)},
            {"access", p.access},
            {"age", p.age},
            {"shortcode", p.shortCode},
            {"safeName", p.safeName},
            {"providerDetails", {}}
    };
}

PlayerData tag_invoke(json::value_to_tag<PlayerData>, json::value const& jv)
{
    return {
        jv.at("nkapiID").as_string().c_str(),
        jv.at("displayName").as_string().c_str(),
        jv.at("shortcode").as_string().c_str(),
        jv.at("safeName").as_string().c_str(),
        jv.at("clan").to_number<int>(),
        jv.at("country").as_string().c_str(),
        jv.at("continent").as_string().c_str(),
        jv.at("avatar").to_number<int>(),
        jv.at("online").as_bool(),
        jv.at("onlineApp").to_number<int>(),
        json::value_to<std::vector<string>>(jv.at("providersAvailable")),
        jv.at("access").to_number<int>(),
        jv.at("age").to_number<int>(),
        true
    };
}

















