//
// Created by Rosier on 2023-01-15.
//

#ifndef BLOONSTD6SERVER_NET_H
#define BLOONSTD6SERVER_NET_H

#include <iostream>

#ifdef _WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

#define close(sock) closesocket(sock)
#define lastError WSAGetLastError()
#define poll(pollfd *fds, unsigned long nfds, int timeout) WSAPoll(fds, nfds, timeout)

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <netinet/tcp.h>
#include <unistd.h>
#include <poll.h>

#define SocketType int
#define INVALID_SOCKET -1
#define lastError std::strerror(errno)

#endif


#endif //BLOONSTD6SERVER_NET_H
