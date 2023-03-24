//
// Created by Rosier on 2023-03-09.
//

#include "listener.hpp"

#include "session.hpp"

using namespace server::proxy;

// Public
Listener::Listener(net::io_context& Ioc, ssl::context& Ctx, tcp::endpoint Endpoint,
                   std::map<std::string, tcp::resolver::results_type>& NkEndpoints)
    : Ioc_(Ioc), Ctx_(Ctx), Acceptor_(Ioc), NkEndpoints_(NkEndpoints) {
    beast::error_code Ec;

    Acceptor_.open(Endpoint.protocol(), Ec);
    if (Ec) {
        beastFail(Ec, "open");
        return;
    }

    Acceptor_.set_option(net::socket_base::reuse_address(true), Ec);
    if (Ec) {
        beastFail(Ec, "set_option");
        return;
    }

    Acceptor_.bind(Endpoint, Ec);
    if (Ec) {
        beastFail(Ec, "bind");
        return;
    }

    Acceptor_.listen(net::socket_base::max_listen_connections, Ec);
    if (Ec) {
        beastFail(Ec, "listen");
        return;
    }
}

void Listener::run() {
    accept();
}

// Private
void Listener::accept() {
    Acceptor_.async_accept(net::make_strand(Ioc_),
                           beast::bind_front_handler(&Listener::onAccept,
                                                     shared_from_this()));
}

void Listener::onAccept(beast::error_code Ec, tcp::socket Socket) {
    if (Ec) {
        return beastFail(Ec, "accept");
    }

    std::make_shared<Session>(std::move(Socket), Ctx_, NkEndpoints_)->run();

    accept();
}
