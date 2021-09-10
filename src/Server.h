#pragma once

#include <string>
#include <thread>
#include <unordered_map>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include "Config.h"
#include "HandleChain.h"
#include "Handler.h"
#include "Logger.h"
#include "Router.h"
#include "Session.h"

namespace blank
{
    namespace beast = boost::beast;
    namespace http = boost::beast::http;
    namespace net = boost::asio;
    using tcp = boost::asio::ip::tcp;

    class Server
    {
    public:
        Server(Config &conf) : conf_(conf), router_(std::make_shared<Router>()) {}
        ~Server() = default;

    public:
        void run();
        void register_handler(const std::string &path, const http::verb &method, HandlerPtr handler, bool enable_default_middlewares = true);
        void register_chain(const std::string &path, const http::verb &method, HandleChainPtr handle_chain);

    private:
        void setup_logger();
        void listen(net::io_context &ioc, tcp::endpoint ep, net::yield_context yield);

    private:
        Config conf_;
        RouterPtr router_;
    };
};