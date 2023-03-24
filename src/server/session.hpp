//
// Created by Rosier on 2023-03-09.
//

#ifndef BLOONSTOOLS_SRC_SERVER_SESSION_HPP
#define BLOONSTOOLS_SRC_SERVER_SESSION_HPP

#include "error.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/dispatch.hpp>

#include <openssl/ssl.h>

#include <memory>
#include <iostream>
#include <optional>
#include <map>
#include <regex>


namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

namespace server::proxy {

const std::regex kConnectRegex("([\\w\\-._~()!$&'*+,;=]+)(?::(\\d+))?",
                               std::regex_constants::ECMAScript |
                               std::regex_constants::icase);

class Session : public std::enable_shared_from_this<Session> {
  public:
    explicit Session(tcp::socket&& Socket, ssl::context& CaCert,
                     std::map<std::string, tcp::resolver::results_type>& NkEndpoints);
    void run();

  private:
    static const http::response<http::empty_body> kAcceptConnectionResponse_;

    void onRun();
    void onReceiveConnect(beast::error_code Ec, std::size_t BytesReceived);
    void doAccept();
    void onAccept(beast::error_code Ec, std::size_t BytesSent);
    void doHandshake();
    void onHandshake(beast::error_code Ec);
    void doReadClient();
    void onReadClient(beast::error_code Ec, std::size_t BytesTransferred);
    void doWriteServer();
    void onWriteServer(beast::error_code Ec, std::size_t BytesSent);
    void doReadServer();
    void onReadServer(beast::error_code Ec, std::size_t BytesReceived);
    void doWriteClient();
    void onWriteClient(beast::error_code Ec, std::size_t BytesSent);

    void doCloseClient();
    void doCloseServer();
    void onShutdown(beast::error_code Ec);

    void connectToServer(const std::string& ServerName);
    void getPeerCertificate();
    void generateDummyCert(X509* PeerCert);

    static int serverNameCallback(SSL* Ssl, int* Alert, void* Arg);

    ssl::context& CaCertificate_;

    beast::tcp_stream ClientStream_;
    ssl::context DownstreamCtx_{ssl::context::tlsv12_server};
    ssl::context UpstreamCtx_{ssl::context::tlsv12_client};
    std::optional<beast::ssl_stream<beast::tcp_stream>> ClientTlsStream_;
    std::optional<beast::ssl_stream<beast::tcp_stream>> ServerTlsStream_;
    beast::flat_buffer ClientBuffer_;
    beast::flat_buffer ServerBuffer_;

    tcp::resolver Resolver_;

    http::request<http::string_body> Request_{};
    http::response<http::string_body> Response_{};

    std::map<std::string, tcp::resolver::results_type>& NkEndpoints_;
};

} // namespace server::proxy

#endif // BLOONSTOOLS_SRC_SERVER_SESSION_HPP
