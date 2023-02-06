//
// Created by Rosier on 2023-01-15.
//

#include "ClientConnection.h"



// Private
void ClientConnection::processJSM(JSMMessage &msg)
{
    m_id = msg.m_id;
    m_apiId = msg.m_apiId;

}

void ClientConnection::createJSRM()
{
    JSRMMessage msgBody;
    msgBody.m_success = true;
    for (int i{1}; i <= m_gameInstance.m_botClients; i++)
    {
        msgBody.m_playersConnected.push_back(static_cast<uint8_t>(i));
    }
    for (auto &player : m_gameInstance.m_players)
    {
        msgBody.m_playersConnected.push_back(player.m_id);
    }

    m_messageSendQueue.emplace(msgBody);
}

void ClientConnection::processLobbyMsg(LobbyMessage &msg)
{
    if (msg.m_messageType == 1) // Is Player Data
    {
        LobbyMessages::PlayerInfo &playerInfo{std::get<LobbyMessages::PlayerInfo>(msg.m_data)};
        m_player->updateInfo(playerInfo.m_jsonData);

        for (auto player : m_gameInstance.m_players)
        {
            if (player.m_id != m_player->m_id)
            {
                createPlayerInfoMsg(player);
            }
        }
    }
}

void ClientConnection::createPlayerInfoMsg(Player &player)
{
    LobbyMessages::PlayerInfo playerInfo;
    playerInfo.encode(player);
    LobbyMessage msg(playerInfo);
    msg.m_peerId = player.m_id;

    m_messageSendQueue.emplace(msg);
}

void ClientConnection::processMessage(MultiplayerMessage &genericMsg)
{
    if (std::holds_alternative<JSMMessage>(genericMsg.m_body))
    {
        JSMMessage &msg{std::get<JSMMessage>(genericMsg.m_body)};
        processJSM(msg);
        createJSRM();
    }
    else if (std::holds_alternative<ECHOMessage>(genericMsg.m_body))
    {
        ECHOMessage &msg{std::get<ECHOMessage>(genericMsg.m_body)};
        msg.m_isReply = true;
        m_messageSendQueue.emplace(msg);
    }
    else if (std::holds_alternative<LobbyMessage>(genericMsg.m_body))
    {
        LobbyMessage &msg{std::get<LobbyMessage>(genericMsg.m_body)};
        processLobbyMsg(msg);
    }


}

// Public
ClientConnection::ClientConnection(SocketType sock, GameInstance &gameInstance, Player* player) :
                                   m_sock(sock), m_gameInstance(gameInstance), m_player(player)
{
    m_thread = std::thread{&ClientConnection::run, this};
}

ClientConnection::ClientConnection(int sock, GameInstance &gameInstance) :
                                   m_sock(sock), m_gameInstance(gameInstance)
{
    m_thread = std::thread{&ClientConnection::run, this};
}

void ClientConnection::run()
{
    struct pollfd pollFds[1];

    pollFds[0].fd = m_sock;
    pollFds[0].events = POLLIN;

    char buffer[10000];
    string data;
    while (true)
    {
        int numEvents = poll(pollFds, 1, 20);
        if (numEvents > 0) {
            long dataLen = recv(m_sock, buffer, sizeof(buffer), 0);
            data.append(buffer, dataLen);

            unsigned long split;
            while ((split = data.find("Fn")) != string::npos) {
                m_messageRecvQueue.emplace(data.substr(0, split), m_gameInstance.m_inLobbyState, m_sock);
                data.erase(0, split + 2);
            }
        }
        while (!m_messageRecvQueue.empty())
        {
            if (std::holds_alternative<JSMMessage>(m_messageRecvQueue.front().m_body) || m_player) {
                processMessage(m_messageRecvQueue.front());
                m_messageRecvQueue.pop();
            }
        }
        while (!m_messageSendQueue.empty())
        {
            string msgData = m_messageSendQueue.front().getBytes();
            send(m_sock, msgData.data(), msgData.length(), 0);
            m_messageSendQueue.pop();
        }
    }
}











