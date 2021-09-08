#include "BlankHttpServer.h"

namespace blank
{
    void BlankHttpServer::run()
    {
        setup_logger();
        BOOST_LOG_TRIVIAL(info) << "running BlankHTTPServer with config: " << conf_.to_json_string();

        auto addr = net::ip::make_address(conf_.address);
        auto ep = tcp::endpoint{addr, conf_.port};

        net::io_context ioc{conf_.threads};

        net::spawn(ioc,
                   std::bind(
                       &BlankHttpServer::listen,
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

    void BlankHttpServer::register_handler(const std::string &path, const http::verb &method, BlankHttpHandlerPtr handler, bool enable_default_middlewares)
    {
        if (enable_default_middlewares)
        {
            auto chain = std::make_shared<BlankHttpHandleChain>(handler, true);
            router_->add_handler(path, method, chain);
            return;
        }
        router_->add_handler(path, method, handler);
    }

    void BlankHttpServer::register_chain(const std::string &path, const http::verb &method, BlankHttpHandleChainPtr chain)
    {
        router_->add_handler(path, method, chain);
    }

    void BlankHttpServer::setup_logger()
    {
        BlankHttpLogger::init(conf_.log_level);
    }

    void BlankHttpServer::listen(net::io_context &ioc, tcp::endpoint ep, net::yield_context yield)
    {
        beast::error_code ec;

        tcp::acceptor acceptor{ioc};
        acceptor.open(ep.protocol(), ec);
        if (ec)
        {
            BOOST_LOG_TRIVIAL(error) << "acceptor open socket error, detail: " << ec.message();
            return;
        }
        acceptor.set_option(net::socket_base::reuse_address(true), ec);
        if (ec)
        {
            BOOST_LOG_TRIVIAL(error) << "acceptor set socket option error, detail: " << ec.message();
            return;
        }
        acceptor.bind(ep, ec);
        if (ec)
        {
            BOOST_LOG_TRIVIAL(error) << "acceptor bind address error, detail: " << ec.message();
            return;
        }
        acceptor.listen(net::socket_base::max_listen_connections, ec);
        if (ec)
        {
            BOOST_LOG_TRIVIAL(error) << "acceptor listen error, detail: " << ec.message();
            return;
        }

        while (true)
        {
            tcp::socket socket{ioc};
            acceptor.async_accept(socket, yield[ec]);
            if (ec)
            {
                BOOST_LOG_TRIVIAL(error) << "acceptor accept connection error, detail: " << ec.message();
                continue;
            }
            else
            {
                std::chrono::seconds timeout{conf_.timeout};
                auto session = std::make_shared<BlankHttpSession>(std::move(socket), timeout, router_);
                net::spawn(
                    acceptor.get_executor(),
                    std::bind(
                        &BlankHttpSession::handle_session,
                        session,
                        std::placeholders::_1));
            }
        }
    }
};