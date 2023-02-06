#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <memory>
#include <cctype>
#include <cstring>

namespace fs = std::filesystem;

#include "common.h"
#include "Proxy.h"
//#include "Message.h"
#include "MultiplayerServer.h"
// #include "MultiplayerMessages.h"
// #include "LobbyMessage.h"
// #include "ClientConnection.h"


int main() {
//    for (auto const& dirEntry : fs::directory_iterator{"/proc"}) {
//        try {
//            std::stoi(dirEntry.path().filename().string());
//        } catch (...) { continue; }
//
//        char buffer[64];
//        std::ifstream procFile{dirEntry.path().string() + "/stat"};
//        procFile.ignore(25, '(');
//        procFile.getline(buffer, 64, ')');
//        if (strcmp(buffer, "BloonsTD6.exe") == 0) {
//            cout << buffer << endl;
//            procFile.close();
//            break;
//        }
//    }



   Proxy proxy;
   std::shared_ptr<MultiplayerServer> server(new MultiplayerServer(1));
   proxy.m_server = server;

   std::thread proxyThread(&Proxy::run, &proxy);
   server->startListening("127.0.0.1", "1445");

    // using namespace std::string_literals;
    // string data{"\x4e\x4b\x4d\x6c\x74\x01\x31\x00\x03\x00\x00\x00\x4a\x53\x4d\x01"s
    // "\x00\x00\x00\x06\x00\x00\x00\x59\x4c\x48\x56\x44\x46\x18\x00\x00"s \
    // "\x00\x36\x31\x63\x38\x32\x37\x32\x35\x32\x35\x33\x33\x35\x32\x37"s \
    // "\x35\x33\x34\x31\x33\x30\x62\x61\x61\x46\x6e\x4e"s
    // };


    // std::fstream file("/home/sierra/Code/C++/BloonsTD6Server/test/binfull", std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    // std::stringstream data;
    // data << file.rdbuf();
    // file.close();
    //
    // LobbyMessages::PlayerInfo playerInfoMessage(data);
    //
    //
    // cout << playerInfoMessage.m_jsonData << endl << endl;
    //
    // Player player;
    // player.updateInfo(playerInfoMessage.m_jsonData);
    // for (const auto& monkey : player.m_namedMonkeys)
    // {
    //     cout << monkey.second << endl;
    // }
    // player.m_apiId = "61c827252533527534130baa";
    // player.m_name = "Sierra";
    // player.m_safeName = "PeachPanda3185";
    // player.m_level = 150;
    // player.m_chosenHero = "Sauda";
    // player.m_namedMonkeys.emplace("DartMonkey", "Poggery");
    //

    // playerInfoMessage.encode(player);

    // for (auto i : player.m_towerCosmetics)
    // {
    //     cout << i.m_tower << endl;
    // }

    // cout << playerInfoMessage.m_jsonData << endl;

    return 0;
}
