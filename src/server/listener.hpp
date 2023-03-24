//
// Created by Rosier on 2023-03-09.
//

#ifndef BLOONSTOOLS_SRC_SERVER_LISTENER_HPP
#define BLOONSTOOLS_SRC_SERVER_LISTENER_HPP

#include "error.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/dispatch.hpp>

#include <memory>
#include <map>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

namespace server::proxy {

class Listener : public std::enable_shared_from_this<Listener> {
  public:
    Listener(net::io_context& Ioc, ssl::context& Ctx, tcp::endpoint Endpoint,
             std::map<std::string, tcp::resolver::results_type>& NkEndpoints);

    void run();

  private:
    void accept();
    void onAccept(beast::error_code Ec, tcp::socket Socket);

    net::io_context& Ioc_;
    ssl::context& Ctx_;
    tcp::acceptor Acceptor_;

    std::map<std::string, tcp::resolver::results_type>& NkEndpoints_;
};

} // namespace server::proxy

#endif // BLOONSTOOLS_SRC_SERVER_LISTENER_HPP
