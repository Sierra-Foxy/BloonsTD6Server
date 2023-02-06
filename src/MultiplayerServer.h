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
#include <thread>
#include <vector>
#include <cstring>
#include <map>
#include <queue>
#include <utility>

class MultiplayerServer {
// Variables
private:
    SocketType m_listenSocket{};
    std::map<SocketType, string> m_connections;

    std::vector<std::thread> m_messageThreads;
    bool m_messageThreadsInterupt{false};

    bool m_listenThreadInterupt{false};

    std::thread m_recvThread;
    bool m_recvThreadInterupt{false};

    std::mutex m_recvQueueMutex;
    std::queue<MultiplayerMessage> m_recvQueue;

public:
    GameInstance m_gameInstance;
    string m_serverId{std::to_string(std::chrono::steady_clock::now().time_since_epoch().count() % 50)};
    // std::queue<std::shared_ptr<ClientConnection>> m_orphanConnections;

// Functions
private:
    void recvLoop();
    void msgLoop();

    void handleMsg(MultiplayerMessage &genericMsg);
    void handleLobbyMsg(MultiplayerMessage &genericMsg);
    void handleExternalMsg(MultiplayerMessage &genericMsg);

    void onJSM(SocketType sendTo, uint8_t fromPlayer);
    void onPlayerConnect(SocketType sendTo, uint8_t excludedPlayer);
    void onPlayerInfo(SocketType sendTo, uint8_t playerId);

public:
    explicit MultiplayerServer(int numMsgThreads);

    void startListening(const string &addr, const string &port);

    int getNextId();
};


#endif //BLOONSTD6SERVER_MULTIPLAYERSERVER_H
