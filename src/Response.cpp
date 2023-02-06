//
// Created by Rosier on 2023-01-05.
//

#include "Response.h"

// // Private
// void Response::localMatch() {
//     m_jsonMessage = json::parse(m_httpMessage.body()).as_object();
//     string data = m_jsonMessage["data"].as_string().c_str();
//     json::object jsonData = json::parse(data).as_object();
//     json::object matchData = {
//             {"id", "FFFFFF"},
//             {"metadata", {
//                            {"clientVersion", "34.3"},
//                            {"Xr", "False"},
//                            {"Difficulty", "?"},
//                            {"Map", "?"},
//                            {"Mode", "?"},
//                            {"MapDifficulty", "Beginner"},
//                            {"CreatorId", SERVER_USER_ID},
//                            {"IsPublic", "False"},
//                            {"_enforced_player_count", 4},
//                            {"_preferRegions", {"BTD6-NA-E"}},
//                            {"relay_server_ip", "50.66.139.207"},
//                            {"relay_server_port", 1445},
//                            {"relay_server_protocol", "TCP"}
//                    }
//             },
//             {"displayName", SERVER_USER_NAME},
//             {"playerGlobalID", SERVER_USER_ID},
//             {"createdAt", std::time(nullptr) - 1000L},
//             {"fromNow", 24310}
//     };
//     jsonData["matches"].as_array().push_back(matchData);
//     string jsonString = json::serialize(jsonData);
//     m_jsonMessage["data"] = jsonString;
// }
//
// void Response::userSearch(const http::request<http::string_body> &request)
// {
//     json::object jsonRequest = json::parse(request.body()).as_object();
//     string data = jsonRequest["data"].as_string().c_str();
//     json::object jsonData = json::parse(data).as_object();
//     for (int i{0}; i < jsonData["keys"].as_array().size(); i++)
//     {
//         string key = jsonData["keys"].as_array().at(i).as_string().c_str();
//         if (key == SERVER_USER_ID)
//         {
//             json::object userData = {
//                     {"nkapiID", SERVER_USER_ID},
//                     {"displayName", SERVER_USER_NAME},
//                     {"clan", 4},
//                     {"country", "N/A"},
//                     {"continent", "N/A"},
//                     {"avatar", 0},
//                     {"online", false},
//                     {"onlineApp", -1},
//                     {"providersAvailable", {"kloud"}},
//                     {"access", 0},
//                     {"age", 0},
//                     {"shortcode", SERVER_USER_CODE},
//                     {"safeName", SERVER_USER_NAME},
//                     {"providerDetails", {}}
//             };
//
//             jsonData["users"].emplace_object()[SERVER_USER_ID] = userData;
//             string jsonString = json::serialize(jsonData);
//             m_jsonMessage["data"] = jsonString;
//         }
//     }
// }
//
// void Response::publicStats()
// {
//     m_jsonMessage = {
//             {"playerId", SERVER_USER_ID},
//             {"playerRank", 50},
//             {"playerXp", 100000.0},
//             {"avatar", "ProfileAvatar01"},
//             {"banner", "TeamsBannerDeafult"}, // Ninja Kiwi can't spell lmao
//             {"gameCount", 100},
//             {"gamesWon", 100},
//             {"highestRound", 100},
//             {"monkeysPlaced", 100},
//             {"bloonsPopped", 100000},
//             {"camosPopped", 10000},
//             {"leadsPopped", 10000},
//             {"purplesPopped", 10000},
//             {"regrowsPopped", 10000},
//             {"ceramicsPopped", 10000},
//             {"moabsPopped", 10000},
//             {"bfbsPopped", 10000},
//             {"cashEarned", 10000},
//             {"abilitiesUsed", 1000},
//             {"coopBloonsPopped", 100000},
//             {"coopCashGiven", 10000},
//             {"achievementsClaimed", {1, 2}},
//             {"achievements", {1,2,3,4,5,6,7,8,9,10,11,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,67,69,70,71,72,73,74,75,76,77,78,80,81,82,85,86,87,88,89,90,91,93,94,95,96,97,98,99,100,103,104,105,106,107,108,109,110,111,112,113,114,115,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,134,135,136,137,139,140,141,142,143,144,145,147}},
//             {"hiddenAchievements", {64,65,66,68,83,84,92,101,102,116,133,138}},
//             {"hiddenAchievementsClaimed", {64, 65}},
//             {"heroesPlacedData", {}},
//             {"towersPlacedData", {}},
//             {"spMedals", {{"CHIMPS-BLACK", 0}, {"Easy", 0}, {"Medium", 0}, {"Hard", 0}, {"PrimaryOnly", 0}, {"Deflation", 0}, {"MilitaryOnly", 0}, {"Apopalypse", 0}, {"Reverse", 0}, {"MagicOnly", 0}, {"HalfCash", 0}, {"DoubleMoabHealth", 0}, {"AlternateBloonsRounds", 0}, {"Impoppable", 0}, {"Clicks", 0}}}, // Ninja Kiwi can't spell lmao
//             {"coopMedals", {{"CHIMPS-BLACK", 0}, {"Easy", 0}, {"Medium", 0}, {"Hard", 0}, {"PrimaryOnly", 0}, {"Deflation", 0}, {"MilitaryOnly", 0}, {"Apopalypse", 0}, {"Reverse", 0}, {"MagicOnly", 0}, {"HalfCash", 0}, {"DoubleMoabHealth", 0}, {"AlternateBloonsRounds", 0}, {"Impoppable", 0}, {"Clicks", 0}}}, // Ninja Kiwi can't spell lmao
//             {"raceMedals", {}},
//             {"bossMedals", {}},
//             {"bossEliteMedals", {}},
//             {"ctLocalMedals", {}},
//             {"ctGlobalMedals", {}},
//             {"bossBadges", {}},
//             {"dailyRewards", 0},
//             {"savedStats", {{"PlayerStatsGamesPlayed", false}, {"PlayerStatsGamesWon", false}, {"PlayerStatsHighestRound", false}, {"PlayerStatsMonkeysPlaced", false}, {"PlayerStatsPopCount", false}, {"PlayerStatsCoopPopCount", false}, {"PlayerStatsCamoPopped", false}, {"PlayerStatsLeadPopped", false}, {"PlayerStatsPurplePopped", false}, {"PlayerStatsRegrowPopped", false}, {"PlayerStatsCeramicPopped", false}, {"PlayerStatsMoabsPopped", false}, {"PlayerStatsBfbsPopped", false}, {"PlayerStatsZomgsPopped", false}, {"PlayerStatsDdtsPopped", false}, {"PlayerStatsBadsPopped", false}, {"PlayerStatsBloonsLeaked", false}, {"PlayerStatsCashGenerated", false}, {"PlayerStatsCashGifted", false}, {"PlayerStatsAbilitiesUsed", false}, {"PlayerStatsPowersUsed", false}, {"PlayerStatsInstaMonkeysUsed", false}, {"PlayerStatsDailysOpened", false}, {"PlayerStatsChallengesCompleted", false}, {"Achievements", false}, {"HiddenAchievements", false}, {"PlayerStatsOdysseysCompleted", false}, {"PlayerStatsLifetimeTrophies", false}, {"PlayerStatsNecroBloons", false}, {"PlayerStatsTransformingTonics", false}, {"MostExperiencedMonkey", false}, {"MostExperienced Monkey Xp", false}, {"Insta Monkey Collection", false}, {"Collection Chests Opened", false}, {"GoldenBloonsPopped", false}, {"MonkeyTeamWins", false}, {"PlayerStatsHighestRoundCHIMPS", false}, {"PlayerStatsHighestRoundDeflation", false}}},
//             {"namedMonkeyStats", {}},
//             {"savedNamedMonkeyStats", {}},
//             {"statsVersion", 1},
//             {"mostExperiencedMonkey", "DartMonkey"},
//             {"mostExperiencedMonkeyXp", 0},
//             {"instaMonkeyCollection", 0},
//             {"paragonsPurchasedData", {}}
//     };
// }
//
// void Response::matchmakingGet()
// {
//     json::object metaData{
//         {"orchestrationMetadata", {{"MM_VERSION", "0"}}},
//         {"playerCount", 10},
//         {"binaryVersion", "1.7.8"},
//         {"hostname", "btd6-7"},
//         {"referee_enabled", false},
//         {"referee_required", false},
//         {"all_sessions_have_referees", false},
//         {"_country", "us"}
//     };
//     string metaDataStr = json::serialize(metaData);
//
//     json::object data{
//         {"match", {
//             {"matchID", "FFFFFF"},
//             {"playerCount", 1},
//             {"metadata", {
//                 {"relay", {
//                         {"ip", "50.66.139.207"},
//                         {"port", 1445},
//                         {"uuid", "btd6-12"},
//                         {"region", "BTD6-NA-E"},
//                         {"expiresAt", std::time(nullptr) + 2592000L},
//                         {"load", 5},
//                         {"metadata", metaDataStr},
//                         {"draining", false},
//                         {"dnsResolved", true},
//                         {"isBackup", false}
//                 }},
//                 {"clientVersion", "34.3"},
//                 {"Xr", "False"},
//                 {"Difficulty", "?"},
//                 {"Map", "?"},
//                 {"Mode", "?"},
//                 {"MapDifficulty", "Beginner"},
//                 {"CreatorId", SERVER_USER_ID},
//                 {"IsPublic", "False"},
//                 {"_enforced_player_count", 4},
//                 {"_preferRegions", {"BTD6-NA-E"}},
//                 {"relay_server_ip", "50.66.139.207"},
//                 {"relay_server_port", 1445},
//                 {"relay_server_protocol", "TCP"}
//             }}
//         }}
//     };
//
//     m_jsonMessage["data"] = json::serialize(data);
// }
//
// void Response::matchmakingJoin()
// {
//     json::object metaData{
//             {"orchestrationMetadata", {{"MM_VERSION", "0"}}},
//             {"playerCount", 10},
//             {"binaryVersion", "1.7.8"},
//             {"hostname", "btd6-7"},
//             {"referee_enabled", false},
//             {"referee_required", false},
//             {"all_sessions_have_referees", false},
//             {"_country", "us"}
//     };
//     string metaDataStr = json::serialize(metaData);
//
//     json::object data{
//         {"matchID", "FFFFFF"},
//         {"matchName", ""},
//         {"playerCount", 2},
//         {"playerGlobalID", m_manager.m_userId}, // THIS IS TEMPORARY THIS NEEDS TO BE THE ID OF THE CONNECTING PLAYER
//         {"playerLocalID", 2}, // THIS NEEDS TO BE THE ID OF THE CONNECTING PLAYER
//         {"metadata", {
//                 {"relay", {
//                       {"ip", "50.66.139.207"},
//                       {"port", 1445},
//                       {"uuid", "btd6-12"},
//                       {"region", "BTD6-NA-E"},
//                       {"expiresAt", std::time(nullptr) + 2592000L},
//                       {"load", 5},
//                       {"metadata", metaDataStr},
//                       {"draining", false},
//                       {"dnsResolved", true},
//                       {"isBackup", false}
//                     }},
//                 {"clientVersion", "34.3"},
//                 {"Xr", "False"},
//                 {"Difficulty", "?"},
//                 {"Map", "?"},
//                 {"Mode", "?"},
//                 {"MapDifficulty", "Beginner"},
//                 {"CreatorId", SERVER_USER_ID},
//                 {"IsPublic", "False"},
//                 {"_enforced_player_count", 4},
//                 {"_preferRegions", {"BTD6-NA-E"}},
//                 {"relay_server_ip", "50.66.139.207.getmoss.site"},
//                 {"relay_server_port", 1445},
//                 {"relay_server_protocol", "TCP"}
//             }
//         },
//         {"maxPlayers", 4}
//     };
//
//     m_jsonMessage["data"] = json::serialize(data);
// }
//
//
// // Public
// Response::Response(ConnectionManager &connectionManager) : m_manager(connectionManager)
// {
//
// }
//
// void Response::modifyResponse(const http::request<http::string_body> &request)
// {
//     parseHttpMessage();
//     string contentType;
//     if (m_target == "/user/search")
//     {
//         userSearch(request);
//         contentType = "application/json";
//     }
//     else if (m_target == "/matchmaking/find/local")
//     {
//         localMatch();
//         contentType = "application/json";
//     }
//
//     if (request["Host"] == "api.ninjakiwi.com")
//     {
//         sign();
//         m_jsonMessage["error"] = nullptr;
//     }
//     createHttpMessage(contentType);
// }
//
// void Response::createCustom(const http::request<http::string_body> &request)
// {
//     string contentType;
//     if (m_target == (string("/storage/static/11/") + string(SERVER_USER_ID) + string("/public-stats")))
//     {
//         publicStats();
//         contentType = "text/plain";
//     }
//     if (m_target == "/matchmaking/get")
//     {
//         matchmakingGet();
//         contentType = "application/json";
//     }
//     if (m_target == "/matchmaking/join")
//     {
//         matchmakingJoin();
//         contentType = "application/json";
//     }
//
//     if (request["Host"] == "api.ninjakiwi.com")
//     {
//         sign();
//         m_jsonMessage["error"] = nullptr;
//     }
//
//     m_httpMessage.keep_alive(true);
//     m_httpMessage.set(http::field::set_cookie, request[http::field::cookie]);
//     m_httpMessage.chunked(true);
//     createHttpMessage(contentType);
// }
//
// bool Response::shouldModify(const string &body)
// {
//     if (
//         m_target == "/user/search" ||
//         m_target == "/matchmaking/find/local"
//         )
//     {
//         return true;
//     }
//     return false;
// }
//
// bool Response::shouldCreateCustom(const string &body)
// {
//     if (
//         m_target == (string("/storage/static/11/") + string(SERVER_USER_ID) + string("/public-stats"))
//         )
//     {
//         return true;
//     }
//     else if (m_target == "/matchmaking/get" || m_target == "/matchmaking/join")
//     {
//         json::object requestJson = json::parse(json::parse(body).as_object()["data"].as_string()).as_object();
//         if (requestJson["matchID"].as_string() == "FFFFFF")
//         {
//             return true;
//         }
//     }
//     return false;
// }
//
// void Response::createHttpMessage(const string &contentType)
// {
//     m_httpMessage.body() = json::serialize(m_jsonMessage);
//     m_httpMessage.set(http::field::content_type, contentType);
//     m_httpMessage.result(200);
//     m_httpMessage.prepare_payload();
// }
//

