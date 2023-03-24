//
// Created by Rosier on 2023-02-27.
//

#ifndef BLOONSTOOLS_SRC_SERVER_API_PROXY_HPP
#define BLOONSTOOLS_SRC_SERVER_API_PROXY_HPP

#include "api_config.hpp"

#include "listener.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/dispatch.hpp>
#include <openssl/x509.h>
#include <openssl/rsa.h>

#include <utility>
#include <chrono>
#include <memory>
#include <iostream>
#include <map>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

namespace server::proxy {

class ApiProxy {
  public:
    ApiProxy();

    void run();

  private:
    void createCaCert(const std::string& FileLocation);

    ApiConfig Config_;

    std::shared_ptr<Listener> Listener_;

    net::io_context Ioc_;
    ssl::context Ctx_{ssl::context::tlsv12};

    std::map<std::string, tcp::resolver::results_type> NkEndpoints_;

    X509* CaCert;
    EVP_PKEY* CaKey;
};

} // namespace server::proxy

#endif // BLOONSTOOLS_SRC_SERVER_API_PROXY_HPP
