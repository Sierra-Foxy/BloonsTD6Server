//
// Created by Rosier on 2023-01-15.
//

#ifndef BLOONSTD6SERVER_MULTIPLAYERSERVER_H
#define BLOONSTD6SERVER_MULTIPLAYERSERVER_H

#include "common.h"
#include "Net.h"
// #include "ClientConnection.h"
#include "MultiplayerMessages.h"
#include "GameConfig.h"
#include "GameInstance.h"

#include <mutex>
#include <chrono>
#include <memory>
#include <thread>
#include <vector>
#include <cstring>
#include <map>
#include <queue>
#include <utility>

struct ConnectionData {
    uint8_t m_id;
    SocketType m_sock;
    string m_buffer;
    string m_globalId;
    Player* m_player;
};

class MultiplayerServer {
// Variables
private:
    SocketType m_listenSocket{};

    std::vector<std::shared_ptr<ConnectionData>> m_connections;
    std::map<SocketType, std::shared_ptr<ConnectionData>> m_connectionFdMap;
    std::map<uint8_t, std::shared_ptr<ConnectionData>> m_connectionIdMap;

    std::vector<std::thread> m_messageThreads;
    bool m_messageThreadsInterupt{false};

    bool m_listenThreadInterupt{false};

    std::thread m_recvThread;
    bool m_recvThreadInterupt{false};

    std::mutex m_recvQueueMutex;
    std::queue<MessageHeader> m_recvQueue;

public:

    GameInstance m_gameInstance;
    string m_serverId{std::to_string(std::chrono::steady_clock::now().time_since_epoch().count() % 50)};

// Functions
private:
    void recvLoop();
    void msgLoop();

    void handleMsg(MessageHeader &msgHeader);
    void handleLobbyMsg(LobbyMessageHeader &msgHeader);

    void onJSM(JSMMessage &msg);
    void onPlayerInfo(PlayerInfo &msg);

public:
    explicit MultiplayerServer(int numMsgThreads);

    void startListening(const string &addr, const string &port);

    int getNextId();

    void changeMap(const GameInstance& instance);
    void changeMaxPlayers(const GameInstance& instance);
    void changeDifficulty(const GameInstance& instance);
    void changeGameMode(const GameInstance& instance);
    void changeDivision(const GameInstance& instance);
};


#endif //BLOONSTD6SERVER_MULTIPLAYERSERVER_H
