//
// Created by Rosier on 2023-01-15.
//

#include "MultiplayerServer.h"

// Private
void MultiplayerServer::recvLoop()
{
    std::vector<struct pollfd> pfds;
    for (auto& connection : m_connections)
    {
        auto& pfd = pfds.emplace_back();
        pfd.fd = connection->m_sock;
        pfd.events = POLLIN;
    }

    while (!m_recvThreadInterupt)
    {
        if (pfds.size() != m_connections.size())
        {
            pfds.resize(m_connections.size());
            int i{0};
            for (auto& connection : m_connections)
            {
                auto& pfd = pfds[i];
                pfd.fd = connection->m_sock;
                pfd.events = POLLIN;
                i++;
            }
        }

        int numEvents = poll(pfds.data(), pfds.size(), 100);
        if (numEvents > 0)
        {
            for (auto &pfd : pfds)
            {
                if (pfd.revents & POLLIN)
                {
                    char buf[10000];
                    long bytesIn{0};
                    bytesIn = recv(pfd.fd, buf, sizeof(buf), 0);
                    // if (bytesIn == 0)
                    // {
                    //     uint8_t id;
                    //     for (int i{0}; i < m_connections.size(); i++)
                    //     {
                    //         if (m_connections[i]->m_sock == pfd.fd)
                    //         {
                    //             id = m_connections[i]->m_id;
                    //             try {
                    //                 m_connectionFdMap.erase(m_connections[i]->m_sock);
                    //             }
                    //             catch (std::out_of_range &e) {}
                    //             try {
                    //                 m_connectionIdMap.erase(m_connections[i]->m_id);
                    //             }
                    //             catch (std::out_of_range &e) {}
                    //             m_connections.erase(m_connections.begin() + i);
                    //             break;
                    //         }
                    //     }
                    //     close(pfd.fd);
                    //     for (int i{0}; i < m_gameInstance.m_players.size(); i++)
                    //     {
                    //         if (m_gameInstance.m_players[i].m_id == id)
                    //         {
                    //             m_gameInstance.m_players.erase(m_gameInstance.m_players.begin()+i);
                    //         }
                    //     }
                    //
                    //     continue;
                    // }
                    auto connection = m_connectionFdMap[pfd.fd];
                    connection->m_buffer.append(buf, bytesIn);
                    unsigned long split{0};
                    MessageHeader header{connection->m_buffer.substr(0, 16)};
                    if ((split = connection->m_buffer.find("Fn", header.m_messageLength + 8)) != string::npos)
                    {
                        std::stringstream dataStream(connection->m_buffer.substr(0, split));
                        m_recvQueue.emplace(dataStream).m_fromSocket = pfd.fd;
                        connection->m_buffer.erase(0, split + 2);
                    }
                }
            }
        }
    }
    m_recvThreadInterupt = false;
}

void MultiplayerServer::msgLoop()
{
    using namespace std::chrono_literals;
    while (!m_messageThreadsInterupt)
    {
        if (!m_recvQueue.empty())
        {
            std::unique_lock<std::mutex> lock(m_recvQueueMutex);
            MessageHeader msg = std::move(m_recvQueue.front());
            m_recvQueue.pop();
            lock.unlock();
            handleMsg(msg);
        }
        else
        {
            std::this_thread::sleep_for(5ms);
        }
    }
}

void MultiplayerServer::handleMsg(MessageHeader &msgHeader)
{
    if (msgHeader.m_code == "JSM")
    {
        JSMMessage msg(std::move(msgHeader));
        onJSM(msg);
    }
    else if (msgHeader.m_code == "ECHO")
    {
        ECHOMessage msg(std::move(msgHeader));
        msg.m_messageLength = 0;
        msg.m_isReply = true;
        string sendBuf{msg.getBytes()};
        send(msg.m_fromSocket, sendBuf.data(), sendBuf.length(), 0);
    }
    else if (msgHeader.m_code.empty())
    {
        if (m_gameInstance.m_inLobbyState) {
            LobbyMessageHeader msg(std::move(msgHeader));
            handleLobbyMsg(msg);
        }
    }

}

void MultiplayerServer::handleLobbyMsg(LobbyMessageHeader &msgHeader)
{
    if (msgHeader.m_msgType == LobbyMessageHeader::lobbyMsgType::PLAYERINFO)
    {
        PlayerInfo msg(std::move(msgHeader));
        onPlayerInfo(msg);
    }
}

void MultiplayerServer::onJSM(JSMMessage &msg)
{
    auto connection = m_connectionFdMap[msg.m_fromSocket];

    m_connectionIdMap[msg.m_id] = connection;
    connection->m_globalId = msg.m_apiId;

    for (auto& player : m_gameInstance.m_players)
    {
        if (player.m_id == connection->m_id)
        {
            connection->m_player = &player;
        }
    }

    JSRMMessage reply;
    reply.m_success = true;
    for (auto &player : m_gameInstance.m_players)
    {
        reply.m_playersConnected.emplace_back(player.m_id);
    }

    string sendBuf{reply.getBytes()};
    send(msg.m_fromSocket, sendBuf.data(), sendBuf.length(), 0);

}

void MultiplayerServer::onPlayerInfo(PlayerInfo &msg)
{
    for (auto &player : m_gameInstance.m_players)
    {
        if (player.m_id != msg.m_peerId)
        {
            PlayerInfo response{player};
            response.m_peerId = player.m_id;

            string sendBuf{response.getBytes()};
            send(msg.m_fromSocket, sendBuf.data(), sendBuf.length(), 0);
            for (auto &sendingPlayer : m_gameInstance.m_players)
            {
                if (sendingPlayer.m_id == msg.m_peerId && sendingPlayer.m_isConnecting)
                {
                    send(msg.m_fromSocket, sendBuf.data(), sendBuf.length(), 0);
                    break;
                }
            }
        }

    }

    for (auto &player : m_gameInstance.m_players)
    {
        if (player.m_id == msg.m_peerId)
        {
            if (player.m_isConnecting) {
                CoopGameTypeChangedMessage coopInfo{m_gameInstance};
                coopInfo.m_peerId = 1;

                string sendBuf{coopInfo.getBytes()};
                send(msg.m_fromSocket, sendBuf.data(), sendBuf.length(), 0);

                MaxPlayerChangedMessage maxPlayerMsg{m_gameInstance};
                maxPlayerMsg.m_peerId = 1;
                maxPlayerMsg.m_maxPlayers = 4;

                sendBuf = maxPlayerMsg.getBytes();
                send(msg.m_fromSocket, sendBuf.data(), sendBuf.length(), 0);
                player.m_isConnecting = false;
            }
            break;
        }
    }
}

// Public
void MultiplayerServer::startListening(const string &addr, const string &port) {
    struct addrinfo hints{};
    struct addrinfo *serverInfo;
    struct addrinfo *p;
    int yes{1};
    int mss{1500};
    socklen_t sinSize;
    sockaddr_storage clientAddr{};

    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    int rv;

    if ((rv = getaddrinfo(addr.c_str(), port.c_str(), &hints, &serverInfo)) != 0)
    {
        cerr << "getaddrinfo: " << gai_strerror(rv) << endl;
        return;
    }

    for (p = serverInfo; p != nullptr; p = p->ai_next)
    {
        if ((m_listenSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == INVALID_SOCKET)
        {
            cerr << "server: socket: " << lastError << endl;
            continue;
        }

        if (setsockopt(m_listenSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            cerr << "server: setsockopt: " << lastError << endl;
            return;
        }

        if (bind(m_listenSocket, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(m_listenSocket);
            cerr << "server: bind: " << lastError << endl;
        }

        break;
    }

    freeaddrinfo(serverInfo);

    if (listen(m_listenSocket, 10) == -1)
    {
        cerr << "listen: " << lastError << endl;
        return;
    }

    struct pollfd pollFds[1];

    pollFds[0].fd = m_listenSocket;
    pollFds[0].events = POLLIN;

    while(!m_listenThreadInterupt)
    {
        int numEvents = poll(pollFds, 1, 20);
        if (numEvents > 0) {
            sinSize = sizeof(clientAddr);
            SocketType clientSocket = accept(m_listenSocket, (struct sockaddr *) &clientAddr, &sinSize);
            if (clientSocket == INVALID_SOCKET) {
                cerr << "accept: " << lastError << endl;
                continue;
            }

            auto connection = m_connections.emplace_back(std::make_shared<ConnectionData>());
            connection->m_sock = clientSocket;
            m_connectionFdMap.emplace(clientSocket, connection);
        }
    }
    m_listenThreadInterupt = false;

}

int MultiplayerServer::getNextId()
{
    uint8_t ids{0};
    for (const auto &client : m_gameInstance.m_players)
    {
        ids |= 1 << (client.m_id - 1);
    }
    for (int i{0}; i < 8; i++)
    {
        if ((ids & (1 << i)) == 0) { return i + 1; }
    }

    return -1; // Server full
}

MultiplayerServer::MultiplayerServer(int numMsgThreads): m_recvThread{&MultiplayerServer::recvLoop, this}
{
    for (int i{0}; i < numMsgThreads; i++)
    {
        m_messageThreads.emplace_back(&MultiplayerServer::msgLoop, this);
    }

    m_gameInstance.m_server = this;
    m_gameInstance.m_changeMapCb = &MultiplayerServer::changeMap;
    m_gameInstance.m_changeMaxPlayerCb = &MultiplayerServer::changeMaxPlayers;
    m_gameInstance.m_changeDifficultyCb = &MultiplayerServer::changeDifficulty;
    m_gameInstance.m_changeGameModeCb = &MultiplayerServer::changeGameMode;
    m_gameInstance.m_changeDivisionCb = &MultiplayerServer::changeDivision;
}

void MultiplayerServer::changeMap(const GameInstance& instance)
{
    MapChangedMessage msg(instance);
    msg.m_peerId = 1;
    string sendBuf{msg.getBytes()};
    for (auto& connection : m_connections)
    {
        send(connection->m_sock, sendBuf.data(), sendBuf.length(), 0);
    }
}

void MultiplayerServer::changeMaxPlayers(const GameInstance& instance)
{
    MaxPlayerChangedMessage msg(instance);
    msg.m_peerId = 1;
    string sendBuf{msg.getBytes()};
    for (auto& connection : m_connections)
    {
        send(connection->m_sock, sendBuf.data(), sendBuf.length(), 0);
    }
}

void MultiplayerServer::changeDifficulty(const GameInstance& instance)
{
    DifficultyChangedMessage msg(instance);
    msg.m_peerId = 1;
    string sendBuf{msg.getBytes()};
    for (auto& connection : m_connections)
    {
        send(connection->m_sock, sendBuf.data(), sendBuf.length(), 0);
    }
}

void MultiplayerServer::changeGameMode(const GameInstance& instance)
{
    GameModeChangedMessage msg(instance);
    msg.m_peerId = 1;
    string sendBuf{msg.getBytes()};
    for (auto& connection : m_connections)
    {
        send(connection->m_sock, sendBuf.data(), sendBuf.length(), 0);
    }
}

void MultiplayerServer::changeDivision(const GameInstance& instance)
{
    DivisionChangedMessage msg(instance);
    msg.m_peerId = 1;
    string sendBuf{msg.getBytes()};
    for (auto& connection : m_connections)
    {
        send(connection->m_sock, sendBuf.data(), sendBuf.length(), 0);
    }
}






















