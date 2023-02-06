//
// Created by Rosier on 2023-01-15.
//

#ifndef BLOONSTD6SERVER_CLIENTCONNECTION_H
#define BLOONSTD6SERVER_CLIENTCONNECTION_H

#include "MultiplayerMessages.h"
#include "GameInstance.h"
#include "Net.h"

#include <cstdio>
#include <queue>
#include <thread>
#include <vector>

class Player;

class ClientConnection {
// Variables
private:
    SocketType m_sock;
    GameInstance &m_gameInstance;
    std::thread m_thread;

    std::queue<MultiplayerMessage> m_messageRecvQueue;
    std::queue<MultiplayerMessage> m_messageSendQueue;

public:
    Player* m_player{};

    uint8_t m_id{0};
    string m_apiId;

// Functions
private:
    void processJSM(JSMMessage &msg);

    void createJSRM();

    void processLobbyMsg(LobbyMessage &msg);
    void createPlayerInfoMsg(Player &player);

    void processMessage(MultiplayerMessage &genericMsg);

public:
    ClientConnection(SocketType sock, GameInstance &gameInstance, Player* player);
    ClientConnection(SocketType sock, GameInstance &gameInstance);

    void run();
};


#endif //BLOONSTD6SERVER_CLIENTCONNECTION_H
