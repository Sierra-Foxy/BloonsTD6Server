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
        http::request<http::string_body> request{};
        http::response<http::string_body> response{};

        try {
            request = m_clientConnection.receiveRequest();
        }
        catch (std::runtime_error &er)
        {
            break;
        }

        m_serverConnection.sendMessage(request);
        cout << "REQUEST:" << endl << request << endl << endl;

        response = m_serverConnection.receiveResponse();
        if (isGzipEncoded(response))
        {
            response.body() = decompressGzip(response.body());
            response.erase(http::field::content_encoding);
        }
        if (isDgdataEncoded(response.body())) {
            cout << decodeDgdata(response.body()) << endl;
        }
        m_clientConnection.sendMessage(response);
        cout << "RESPONSE:" << endl << response << endl << endl;
    }
}





