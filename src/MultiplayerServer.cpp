//
// Created by Rosier on 2023-01-15.
//

#include "MultiplayerServer.h"

// Private
void MultiplayerServer::recvLoop()
{
    std::vector<struct pollfd> pfds;
    for (auto &connection : m_connections)
    {
        auto pfd = pfds.emplace_back();
        pfd.fd = connection.first;
        pfd.events = POLLIN;
    }

    while (!m_recvThreadInterupt)
    {
        if (pfds.size() != m_connections.size())
        {
            pfds.resize(m_connections.size());
            int i{0};
            for (auto &connection : m_connections)
            {
                auto &pfd = pfds[i];
                pfd.fd = connection.first;
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
                    auto &connection = m_connections[pfd.fd];
                    connection.append(buf, bytesIn);
                    // cout << connection << endl;
                    unsigned long split{0};
                    MessageHead head;
                    head.decode(connection.substr(0, 16));
                    if ((split = connection.find("Fn", head.m_messageLength + 8)) != string::npos)
                    {
                        m_recvQueue.emplace(connection.substr(0, split), m_gameInstance.m_inLobbyState, pfd.fd);
                        connection.erase(0, split + 2);
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
            MultiplayerMessage msg = std::move(m_recvQueue.front());
            m_recvQueue.pop();
            lock.unlock();
            handleMsg(msg);
        }
        else
        {
            std::this_thread::sleep_for(20ms);
        }
    }
}

void MultiplayerServer::handleMsg(MultiplayerMessage &genericMsg)
{
    cout << "Message Code: " << genericMsg.m_head.m_code << endl;
    if (std::holds_alternative<JSMMessage>(genericMsg.m_body))
    {
        auto &msg = std::get<JSMMessage>(genericMsg.m_body);
        onJSM(genericMsg.m_parentSocket, msg.m_id);
    }

    else if (std::holds_alternative<ECHOMessage>(genericMsg.m_body))
    {
        auto replyBody = std::get<ECHOMessage>(genericMsg.m_body);
        replyBody.m_isReply = true;
        MultiplayerMessage reply{replyBody};
        string replyBuffer{reply.getBytes()};
        send(genericMsg.m_parentSocket, replyBuffer.data(), replyBuffer.length(), 0);
    }

    else if (std::holds_alternative<LobbyMessage>(genericMsg.m_body))
    {
        handleLobbyMsg(genericMsg);
    }
}

void MultiplayerServer::handleLobbyMsg(MultiplayerMessage &genericMsg)
{
    auto &lobbyMsg = std::get<LobbyMessage>(genericMsg.m_body);
    if (lobbyMsg.m_messageType == 0)
    {
        handleExternalMsg(genericMsg);
    }
    if (lobbyMsg.m_messageType == 1)
    {
        auto &msg = std::get<LobbyMessages::PlayerInfo>(lobbyMsg.m_data);
        onPlayerInfo(genericMsg.m_parentSocket, lobbyMsg.m_peerId);
    }
}

void MultiplayerServer::handleExternalMsg(MultiplayerMessage &genericMsg)
{

}

void MultiplayerServer::onJSM(SocketType sendTo, uint8_t fromPlayer)
{
    JSRMMessage replyBody;
    replyBody.m_success = true;
    for (auto &player : m_gameInstance.m_players)
    {
        replyBody.m_playersConnected.emplace_back(player.m_id);
    }

    MultiplayerMessage reply{replyBody};
    string replyBuffer{reply.getBytes()};
    send(sendTo, replyBuffer.data(), replyBuffer.length(), 0);

    // onPlayerConnect(sendTo, fromPlayer);
}

void MultiplayerServer::onPlayerConnect(SocketType sendTo, uint8_t excludedPlayer)
{
    for (auto &player : m_gameInstance.m_players)
    {
        if (player.m_id != excludedPlayer)
        {
            LobbyMessages::PlayerInfo pInfo;
            pInfo.encode(player);

            LobbyMessage msgBody(std::move(pInfo));
            MultiplayerMessage msg(std::move(msgBody));

            string msgBuffer{msg.getBytes()};
            send(sendTo, msgBuffer.data(), msgBuffer.length(), 0);
        }
    }

}

void MultiplayerServer::onPlayerInfo(int sendTo, uint8_t playerId)
{
    // for (auto &player : m_gameInstance.m_players)
    // {
    //     if (player.m_id != playerId)
    //     {
    //         LobbyMessages::PlayerInfo pInfo;
    //         pInfo.encode(player);
    //
    //         LobbyMessage msgBody(std::move(pInfo));
    //         MultiplayerMessage msg(std::move(msgBody));
    //
    //         string msgBuffer{msg.getBytes()};
    //         send(sendTo, msgBuffer.data(), msgBuffer.length(), 0);
    //     }
    // }

    for (auto &player : m_gameInstance.m_players)
    {
        if (player.m_id == playerId && player.m_isConnecting)
        {
            {
                for (auto &p : m_gameInstance.m_players)
                {
                    if (p.m_id != playerId)
                    {
                        LobbyMessages::PlayerInfo pInfo;
                        pInfo.encode(p);

                        LobbyMessage msgBody(std::move(pInfo));
                        msgBody.m_peerId = p.m_id;
                        MultiplayerMessage msg(std::move(msgBody));

                        string msgBuffer{msg.getBytes()};
                        send(sendTo, msgBuffer.data(), msgBuffer.length(), 0);
                        send(sendTo, msgBuffer.data(), msgBuffer.length(), 0);
                    }
                }
            }
            {
                CoopGameTypeChangedMessage coopInfo{m_gameInstance};

                LobbyMessage msgBody{ExternalMessage{coopInfo}};
                msgBody.m_peerId = 1;
                MultiplayerMessage msg{msgBody};
                string msgBuffer{msg.getBytes()};
                send(sendTo, msgBuffer.data(), msgBuffer.length(), 0);
                player.m_isConnecting = false;
                break;
            }
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

        // if (setsockopt(m_listenSocket, IPPROTO_TCP, TCP_MAXSEG, &mss, sizeof(int)) == -1)
        // {
        //     cerr << "server: setsockopt: " << lastError << endl;
        // }

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

            m_connections[clientSocket];
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
}






















