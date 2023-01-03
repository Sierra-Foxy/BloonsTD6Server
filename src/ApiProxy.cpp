//
// Created by Rosier on 2022-12-19.
//

#include "ApiProxy.h"

void ApiProxy::printMgStr(mg_str str) {
    for (int i{0}; i < str.len; i++) {
        cout << str.ptr[i];
    }
    cout << endl;
}

void ApiProxy::createCert(X509 *&certificate, EVP_PKEY *&pkey, const string &subjectName, bool selfSign)
{
    pkey = EVP_RSA_gen(2048);
    certificate = X509_new();

    std::mt19937 mt{ static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count()) };

    ASN1_INTEGER_set(X509_get_serialNumber(certificate), mt());

    X509_gmtime_adj(X509_get_notBefore(certificate), 0);
    X509_gmtime_adj(X509_get_notAfter(certificate), 60 * 60 * 24 * 365 * 10);

    X509_set_pubkey(certificate, pkey);

    X509_NAME *name;
    name = X509_get_subject_name(certificate);
    X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char *)subjectName.c_str(), -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)subjectName.c_str(), -1, -1, 0);

    if (selfSign) {
        X509_set_issuer_name(certificate, name);
        X509_sign(certificate, pkey, EVP_sha256());
        return;
    }
    else {
        X509_NAME *issuerName;
        issuerName = X509_NAME_dup(X509_get_subject_name(m_ca));
        X509_set_issuer_name(certificate, issuerName);

        X509_sign(certificate, m_caPKey, EVP_sha256());
    }
}

void ApiProxy::createTmpCert(std::string &certDir, std::string &keyDir)
{
    X509 *cert{nullptr};
    EVP_PKEY *pKey{nullptr};

    cout << "create cert" << endl;

    createCert(cert, pKey, "BloonsTD6Server", false);

    cout << "create name" << endl;
    certDir = std::tmpnam(nullptr);
    keyDir = std::tmpnam(nullptr);

    cout << "open files" << endl;
    std::FILE *certFile = std::fopen(certDir.c_str(), "w+");
    std::FILE *keyFile = std::fopen(keyDir.c_str(), "w+");

    cout << "write files" << endl;
    cout << "write x509" << endl;
    PEM_write_X509(certFile, cert);
    cout << "write private key" << endl;
    PEM_write_PrivateKey(keyFile, pKey, nullptr, nullptr, 0, nullptr, nullptr);

    cout << "close files" << endl;
    std::fclose(certFile);
    std::fclose(keyFile);
}

void *ApiProxy::getInAddr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void ApiProxy::runProxy()
{
    mg_mgr_init(&m_manager);

    int fd = mg_mkpipe(&m_manager, ApiProxy::ipcHandler, this, true);
    m_listenerThread = std::thread(ApiProxy::listenForClients, fd);

    while (true) {
        if (cont) {
            mg_mgr_poll(&m_manager, 1000);
        }
    }
    mg_mgr_free(&m_manager);
}

void ApiProxy::listenForClients(int ipcFd)
{
    int retValue;
    int yes{1};
    int sockfd, clientfd;
    struct addrinfo hints, *serverInfo, *p;
    struct sockaddr_storage clientAddr;
    socklen_t sinSize;
    char s[INET6_ADDRSTRLEN];

    struct mg_connection *connection;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((retValue = getaddrinfo("0.0.0.0", "44333", &hints, &serverInfo)) != 0) {
        cout << "getaddrinfo(): " << gai_strerror(retValue) << endl;
    }

    for (p = serverInfo; p != nullptr; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            std::perror("Server: socket()");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            std::perror("setsockopt()");
            continue;
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            std::perror("Server: bind()");
            continue;
        }

        break;
    }

    freeaddrinfo(serverInfo);

    if (p == nullptr) {
        std::perror("Server: Failed to bind");
        exit(1);
    }

    if (listen(sockfd, 3) == -1) {
        std::perror("listen()");
        exit(1);
    }

    while (true) {
        sinSize = sizeof(clientAddr);
        clientfd = accept(sockfd, (struct sockaddr *) &clientAddr, &sinSize);

        inet_ntop(clientAddr.ss_family, getInAddr((struct sockaddr *)&clientAddr), s, sizeof(s));

        if (clientfd == -1) {
            std::perror("accept()");
            continue;
        }

        send(ipcFd, &clientfd, sizeof(int), 0);
    }
}

void ApiProxy::clientHandler(struct mg_connection *connection, int event, void *eventData, void *functionData) {
    cout << event << endl;
    if (event == MG_EV_ACCEPT)
    {
        ApiProxy *object = (ApiProxy *)functionData;

        EVP_PKEY *serverPKey;
        X509 *serverCert;

        OSSL_ENCODER_CTX *encoderCtx;
        OSSL_ENCODER *x509Encoder;

        BIO *bioBuffer;
        char *keyData{nullptr};
        char *certData{nullptr};
        size_t keyDataLen;
        size_t certDataLen;

//        object->createCert(serverCert, serverPKey, "BloonsTD6Server", false);
        bioBuffer = BIO_new(BIO_s_mem());
        PEM_write_bio_PrivateKey(bioBuffer, serverPKey, nullptr, nullptr, 0, nullptr, nullptr);

        BIO_write(bioBuffer, "\0\0", 1);
        BIO_get_mem_data(bioBuffer, &keyData);

        BIO_write(bioBuffer, "\0\0", 1);
        PEM_write_bio_X509(bioBuffer, serverCert);
        BIO_get_mem_data(bioBuffer, &certData);

//        object->cont = false;
        struct mg_tls_opts opts = {
                .cert = object->m_caDir.c_str(),
                .certkey = object->m_caKeyDir.c_str()
        };
        mg_tls_init(connection, &opts);
    }
    else if (event == MG_EV_ERROR)
    {
        cout << (char *)eventData << endl;
    }
    else if (event == MG_EV_READ)
    {
        for (int i{0}; i < connection->recv.len; i++) {
            cout << connection->recv.buf[i];
        }
        cout << endl;
    }
    else if (event == MG_EV_HTTP_MSG)
    {
//        auto *message = (struct mg_str *) eventData;
//        for (int i{0}; i < message->len; i++) {
//            cout << message->ptr[i];
//        }
//
//        cout << endl;
//
//        struct mg_http_message httpMessage{};
//        mg_http_parse(message->ptr, message->len, &httpMessage);
//
//        printMgStr(httpMessage.uri);

        mg_http_reply(connection, 200, nullptr, "I am so evil and gay");

//        for (auto header : httpMessage.headers) {
//            if (mg_vcmp(&header.name, "Host") == 0) {
//                printMgStr(header.name);
//                printMgStr(header.value);
//            }
//            else {
//                continue;
//            }
//        }
    }
}

void ApiProxy::ipcHandler(struct mg_connection *connection, int event, void *eventData, void *functionData)
{
    auto instance = (ApiProxy *) functionData;
    if (event == MG_EV_READ){
        int clientfd;
        std::memcpy(&clientfd, connection->recv.buf, sizeof(int));

        int msgSize = recv(clientfd, nullptr, 2000, MSG_PEEK | MSG_TRUNC);
        unsigned char buffer[msgSize + 1];
        recv(clientfd, buffer, sizeof(buffer), MSG_PEEK);
        for (int i{0}; i < sizeof(buffer); i++) {
            if (i % 16 == 0) { cout << endl; }
            else if (i % 8 == 0) { cout << ' '; }
            cout << std::hex << std::setw(2) << std::setfill('0') << (int)buffer[i] << ' ';
        }
        cout << endl;

        connection = mg_wrapfd(&instance->m_manager, clientfd, ApiProxy::clientHandler, instance);

        string certDir;
        string keyDir;
        instance->createTmpCert(certDir, keyDir);

        struct mg_tls_opts opts = {
                .cert = certDir.c_str(),
                .certkey = keyDir.c_str()
        };


        mg_tls_init(connection, &opts);
    }
}

ApiProxy::ApiProxy(const string &caDir, const string &caKeyDir) : m_caDir(caDir), m_caKeyDir(caKeyDir) {
    // OpenSSL initialisation
    ERR_load_crypto_strings();
    OpenSSL_add_all_algorithms();

    std::FILE *caFile;
    std::FILE *caKeyFile;

    caFile = std::fopen(m_caDir.c_str(), "r+");
    caKeyFile = std::fopen(m_caKeyDir.c_str(), "r+");

    if (caFile == nullptr || caKeyFile == nullptr) {
        if (caFile != nullptr) {
            std::fclose(caFile);
        }
        if (caKeyFile != nullptr) {
            std::fclose(caKeyFile);
        }

        caFile = std::fopen(m_caDir.c_str(), "w+");
        caKeyFile = std::fopen(m_caKeyDir.c_str(), "w+");

        createCert(m_ca, m_caPKey, "BloonsTD6ServerCA", true);

        PEM_write_PrivateKey(caKeyFile, m_caPKey, nullptr, nullptr, 0, nullptr, nullptr);
        PEM_write_X509(caFile, m_ca);
        std::fclose(caFile);
        std::fclose(caKeyFile);
    }
    else {
        m_ca = PEM_read_X509(caFile, &m_ca, nullptr, nullptr);
        m_caPKey = PEM_read_PrivateKey(caKeyFile, &m_caPKey, nullptr, nullptr);
    }

}

ApiProxy::~ApiProxy() {
    X509_free(m_ca);
    EVP_PKEY_free(m_caPKey);
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();
    ERR_free_strings();
}




