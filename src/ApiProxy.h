//
// Created by Rosier on 2022-12-19.
//

#ifndef BLOONSTD6SERVER_APIPROXY_H
#define BLOONSTD6SERVER_APIPROXY_H

#include <thread>
#include <cstring>
#include <iomanip>
#include <random>
#include <chrono>
#include <ctime>
#include <cstdio>

#include <openssl/bio.h>
#include <openssl/params.h>
#include <openssl/core.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/x509.h>
#include <openssl/pem.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "common.h"

using namespace std::chrono_literals;

class ApiProxy {
private:
    X509 *m_ca{nullptr};
    EVP_PKEY *m_caPKey{nullptr};

    X509 *m_serverCert{nullptr};
    EVP_PKEY *m_serverPKey{nullptr};

    string m_caDir;
    string m_caKeyDir;

    struct mg_mgr m_manager;

    bool cont{true};

    std::thread m_listenerThread;

private:
    static void printMgStr(mg_str str);

    void createCert(X509 *&certificate, EVP_PKEY *&pkey, const string &subjectName, bool selfSign);
    void createTmpCert(std::string &certDir, std::string &keyDir);

    void sniffUpstreamCertificate(char *data, size_t len);
    static void *getInAddr(struct sockaddr *sa);

    static void listenForClients(int ipcFd);

public:

public:
    ApiProxy(const string &caDir, const string &caKeyDir);
    virtual ~ApiProxy();

    void runProxy();

    static void clientHandler(struct mg_connection *connection, int event, void *eventData, void *functionData);
    static void ipcHandler(struct mg_connection *connection, int event, void *eventData, void *functionData);
};


#endif //BLOONSTD6SERVER_APIPROXY_H
