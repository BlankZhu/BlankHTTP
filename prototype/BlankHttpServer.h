#pragma once

#include <string>
#include <thread>
#include <unordered_map>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include "BlankHttpConfig.h"
#include "BlankHttpHandleChain.h"
#include "BlankHttpHandler.h"
#include "BlankHttpLogger.h"
#include "BlankHttpRouter.h"
#include "BlankHttpSession.h"

namespace blank
{
    namespace beast = boost::beast;
    namespace http = boost::beast::http;
    namespace net = boost::asio;
    using tcp = boost::asio::ip::tcp;

    class BlankHttpServer
    {
    public:
        BlankHttpServer(BlankHttpConfig &conf) : conf_(conf), router_(std::make_shared<BlankHttpRouter>()) {}
        ~BlankHttpServer() = default;

    public:
        void run();
        void register_handler(const std::string &path, BlankHttpHandlerPtr handler, bool enable_default_middlewares = true);
        void register_chain(const std::string &path, BlankHttpHandleChainPtr handle_chain);

    private:
        void setup_logger();
        void listen(net::io_context &ioc, tcp::endpoint ep, net::yield_context yield);

    private:
        BlankHttpConfig conf_;
        BlankHttpRouterPtr router_;
    };
};