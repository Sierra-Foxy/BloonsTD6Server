//
// Created by Rosier on 2023-01-02.
//

#include "Endpoint.h"

// Private
void Endpoint::localMatch(const string &dataStr) {
    cout << dataStr << endl;
    m_jsonMessage = json::parse(dataStr).as_object();
    string data = m_jsonMessage["data"].as_string().c_str();
    json::object jsonData = json::parse(data).as_object();
    json::object matchData = {
            {"id", "FFFFFF"},
            {"metadata", {
                           {"clientVersion", "34.0_34.1_34.3"},
                           {"Xr", "False"},
                           {"Difficulty", "?"},
                           {"Map", "?"},
                           {"Mode", "?"},
                           {"MapDifficulty", "Beginner"},
                           {"CreatorId", "ffffffff"},
                           {"IsPublic", "False"},
                           {"_enforced_player_count", 4},
                           {"_preferRegions", {"BTD6-NA-E"}},
                           {"relay_server_ip", "5.161.142.158"},
                           {"relay_server_port", 1445},
                           {"relay_server_protocol", "TCP"}
                   }
            },
            {"displayName", "SierraFoxy"},
            {"playerGlobalID", "ffffffff"},
            {"createdAt", 1670197220807},
            {"fromNow", 24310}
    };
    jsonData["matches"].as_array().push_back(matchData);
    string jsonString = json::serialize(jsonData);
    m_jsonMessage["data"] = jsonString;
}

void Endpoint::userSearch(const string &request, const string &response)
{
    json::object jsonRequest = json::parse(request).as_object();
    string data = jsonRequest["data"].as_string().c_str();
    json::object jsonData = json::parse(data).as_object();
    for (int i{0}; i < jsonData["keys"].as_array().size(); i++)
    {
        string key = jsonData["keys"].as_array().at(i).as_string().c_str();
        if (key == "ffffffff")
        {
            json::object userData = {
                    {"nkapiID", "ffffffff"},
                    {"displayName", "BloonsTD6Server"},
                    {"clan", 4},
                    {"country", "N/A"},
                    {"continent", "N/A"},
                    {"avatar", 0},
                    {"online", false},
                    {"onlineApp", -1},
                    {"providersAvailable", {"gp", "kloud"}}, // What is this?
                    {"access", 0},
                    {"age", 0},
                    {"shortcode", "FFFFFFFFF"},
                    {"safeName", "BloonsTD6Server"},
                    {"providerDetails", {}}
            };

            jsonData["users"].emplace_object()["ffffffff"] = userData;
            string jsonString = json::serialize(jsonData);
            m_jsonMessage["data"] = jsonString;
        }
    }
}

// Public

void Endpoint::makeResponse(const string &target, const string &request, const string &response)
{
    if (target == "/matchmaking/find/local")
    {
        localMatch(response);
    }
    else if (target == "/user/search")
    {
        userSearch(request, response);
    }

    sign();
    m_jsonMessage["error"] = nullptr;
}

void Endpoint::makeRequest(const string &nonce)
{
    m_jsonMessage["nonce"] = nonce;
}

void Endpoint::sign() {
    m_jsonMessage["sig"] = hmac(m_jsonMessage["data"].get_string().c_str(), "A26308E242742374");
}

//void Endpoint::escapeString(string &input)
//{
//
//    for(unsigned int i{0}; i < input.size(); i++)
//    {
//        if (input[i] == '\\')
//        {
//            input.replace(i, 1, R"(\\)");
//            i += 2;
//        }
//        if (input[i] == '\"')
//        {
//            input.replace(i, 1, R"(\")");
//            i++;
//        }
//    }
//}
//
//void Endpoint::unescapeString(string &input) {
//    for (int i = input.size(); i >= 0; i--)
//    {
//        if (input[i] == '\\' && input[i + 1] == '\\')
//        {
//            input.erase(i);
//        }
//        else if (input[i] == '\\' && input[i + 1] == '\"')
//        {
//            input.erase(i);
//            i--;
//        }
//    }
//}



string Endpoint::str() {
    return json::serialize(m_jsonMessage);
}


