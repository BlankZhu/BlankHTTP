#include "Server.h"

namespace blank
{
    void Server::run()
    {
        setup_logger();
        logger_.info(fmt("running BlankHTTPServer with config: %1%") % conf_.to_json_string());

        auto addr = net::ip::make_address(conf_.address);
        auto ep = tcp::endpoint{addr, conf_.port};

        net::io_context ioc{conf_.threads};

        net::spawn(ioc,
                   std::bind(
                       &Server::listen,
                       this,
                       std::ref(ioc),
                       ep,
                       std::placeholders::_1));

        std::vector<std::thread> threads;
        for (auto i = 0; i < conf_.threads - 1; i += 1)
        {
            threads.emplace_back([&ioc]
                                 { ioc.run(); });
        }
        // block here
        ioc.run();
    }

    void Server::register_handler(const std::string &path, const http::verb &method, HandlerPtr handler, bool enable_default_middlewares)
    {
        if (enable_default_middlewares)
        {
            auto chain = std::make_shared<HandleChain>(handler, true);
            router_->add_handler(path, method, chain);
            return;
        }
        router_->add_handler(path, method, handler);
    }

    void Server::register_chain(const std::string &path, const http::verb &method, HandleChainPtr chain)
    {
        router_->add_handler(path, method, chain);
    }

    void Server::setup_logger()
    {
        logger_.init(conf_.log_level, conf_.log_filename);
    }

    void Server::listen(net::io_context &ioc, tcp::endpoint ep, net::yield_context yield)
    {
        beast::error_code ec;

        tcp::acceptor acceptor{ioc};
        acceptor.open(ep.protocol(), ec);
        if (ec)
        {
            logger_.error(fmt("acceptor failed to open socket, detail: [%1%]") % ec.message());
            return;
        }
        acceptor.set_option(net::socket_base::reuse_address(true), ec);
        if (ec)
        {
            logger_.error(fmt("acceptor failed to set socket option, detail: [%1%]") % ec.message());
            return;
        }
        acceptor.bind(ep, ec);
        if (ec)
        {
            logger_.error(fmt("acceptor failed to bind address, detail: [%1%]") % ec.message());
            return;
        }
        acceptor.listen(net::socket_base::max_listen_connections, ec);
        if (ec)
        {
            logger_.error(fmt("acceptor failed to listen connection, detail: [%1%]") % ec.message());
            return;
        }

        while (true)
        {
            tcp::socket socket{ioc};
            acceptor.async_accept(socket, yield[ec]);
            if (ec)
            {
                logger_.error(fmt("acceptor failed to accept connection, detail: [%1%]") % ec.message());
                continue;
            }
            else
            {
                std::chrono::seconds timeout{conf_.timeout};
                auto session = std::make_shared<Session>(std::move(socket), timeout, router_);
                net::spawn(
                    acceptor.get_executor(),
                    std::bind(
                        &Session::handle_session,
                        session,
                        conf_,
                        std::placeholders::_1));
            }
        }
    }
};