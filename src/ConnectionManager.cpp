//
// Created by Rosier on 2022-12-31.
//

#include "ConnectionManager.h"

#include <utility>

// Private
bool ConnectionManager::shouldFilterRequest() {
    return false;
}

bool ConnectionManager::shouldFilterResponse() {
    return false;
}

template <bool isRequest>
bool ConnectionManager::isGzipEncoded(const http::message<isRequest, http::string_body> &msg)
{
    std::string_view contentEncoding;
    try {
        contentEncoding = msg.at(http::field::content_encoding);
    }
    catch (std::out_of_range &)
    {
        return false;
    }

    if (contentEncoding == "gzip") { return true; }
    return false;
}


string ConnectionManager::decompressGzip(const string &data)
{
    std::istringstream compressed(data);

    bio::filtering_istreambuf in;
    in.push(bio::gzip_decompressor());
    in.push(compressed);

    std::ostringstream origin;
    bio::copy(in, origin);
    return origin.str();
}

bool ConnectionManager::modifyResponse(const string &target)
{
    if (target == "/user/search")
    {
        return true;
    }
    else if (target == "/matchmaking/find/local")
    {
        return true;
    }
    return false;
}

bool ConnectionManager::customResponse(const string &target)
{

    return false;
}

// Public

ConnectionManager::ConnectionManager(net::io_context &ioc, std::shared_ptr<boost::asio::basic_stream_socket<tcp>> acceptedSocket,
                                     X509 *caCert, EVP_PKEY *caKey) :
                                    m_ioc(ioc), m_clientConnection(ioc), m_serverConnection(ioc), m_caCert(caCert), m_caKey(caKey)
{
    m_clientConnection.m_socket = std::move(acceptedSocket);
}

void ConnectionManager::run()
{
    string sni = m_clientConnection.getSNI(m_clientConnection.getClientHello());
    if (m_blockBFB && sni == "bfb.ninjakiwi.com")
    {
        cout << "blocked BFB" << endl;
        return;
    }

    tcp::endpoint endpoint{net::ip::make_address(m_clientConnection.getOriginalDest()), 443};
    m_serverConnection.initClient(endpoint, sni);

    X509 *upstreamCert = m_serverConnection.getCertificate();
    m_clientConnection.createCertificate(m_caCert, m_caKey, upstreamCert);

    m_clientConnection.initServer();

    while(true)
    {
        Request request{};
        Response response{};

        try {
            request.getHttpMessage() = m_clientConnection.receiveRequest();
        }
        catch (std::runtime_error &er)
        {
            break;
        }

        response.m_target = request.getHttpMessage().target();
        cout << "Target: " << request.getHttpMessage().target() << endl;

        if (response.shouldCreateCustom(request.getHttpMessage().body()))
        {
            response.createCustom(request.getHttpMessage());
        }
        else
        {
            m_serverConnection.sendMessage(request.getHttpMessage());

            response.getHttpMessage() = m_serverConnection.receiveResponse();
            if (isGzipEncoded(response.getHttpMessage())) {
                response.getHttpMessage().body() = decompressGzip(response.getHttpMessage().body());
                response.getHttpMessage().erase(http::field::content_encoding);
            }
            if (isDgdataEncoded(response.getHttpMessage().body())) {
                cout << decodeDgdata(response.getHttpMessage().body()) << endl;
            }
            if (response.shouldModify(request.getHttpMessage().body())) {
                response.modifyResponse(request.getHttpMessage());
            }
        }
        m_clientConnection.sendMessage(response.getHttpMessage());

        cout << "REQUEST:" << endl << request.getHttpMessage() << endl << endl;
        cout << "RESPONSE:" << endl << response.getHttpMessage() << endl << endl;
    }
}















