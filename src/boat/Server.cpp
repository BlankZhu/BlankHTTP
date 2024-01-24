#include <boat/Server.h>

#include <boat/log/AsyncLogger.h>
#include <boat/log/SyncLogger.h>

namespace boat {

void Server::init(log::LoggerType type) {
  setup_logger(type);
  initialized_ = true;
}

void Server::run() {
  boost::asio::io_context ioc{static_cast<int>(conf_.get_threads())};
  run(ioc);
}

void Server::run(boost::asio::io_context &ioc) {
  logger_->Info(boost::format("running BlankHTTPServer with config: %1%") %
                conf_.detail_in_json());

  if (conf_.get_enable_ssl()) {
    boost::beast::error_code ec;
    setup_ssl_context(ec);
    if (ec) {
      logger_->Fatal(boost::format("failed to setup SSL, detail: [%1%]") %
                     ec.message());
      return;
    }
  }

  auto addr = boost::asio::ip::make_address(conf_.get_address());
  auto ep = boost::asio::ip::tcp::endpoint{addr, conf_.get_port()};

  boost::asio::spawn(ioc, std::bind(&Server::listen, this, std::ref(ioc), ep,
                                    std::placeholders::_1));

  std::function<void(boost::asio::io_context &)> ioc_run_func;
  std::function<void(boost::asio::io_context &)> ioc_block =
      [](boost::asio::io_context &ioc) { ioc.run(); };
  std::function<void(boost::asio::io_context &)> ioc_spin =
      [](boost::asio::io_context &ioc) {
        while (true) {
          ioc.poll();
        }
      };

  if (conf_.get_enable_spin()) {
    ioc_run_func = ioc_spin;
  } else {
    ioc_run_func = ioc_block;
  }

  boost::thread_group tg{};
  for (unsigned int i = 0; i < conf_.get_threads(); i += 1) {
    tg.create_thread([&ioc, &ioc_run_func]() { ioc_run_func(ioc); });
  }
  tg.join_all();
}

void Server::register_handler(const std::string &path,
                              const boost::beast::http::verb &method,
                              const HandlerPtr &handler,
                              const bool enable_default_middlewares) const {
  if (initialized_) {
    logger_->Warning(boost::format(
        "cannot register new handler once the server is initialized"));
    return;
  }

  if (enable_default_middlewares) {
    const auto chain = std::make_shared<HandleChain>(handler, true);
    router_->add_handler(path, method, chain);
    return;
  }
  router_->add_handler(path, method, handler);
}

void Server::register_chain(const std::string &path,
                            const boost::beast::http::verb &method,
                            const HandleChainPtr &handle_chain) const {
  if (initialized_) {
    logger_->Warning(boost::format(
        "cannot register new chain once the server is initialized"));
    return;
  }

  router_->add_handler(path, method, handle_chain);
}

void Server::setup_logger(const log::LoggerType type) {
  if (type == log::LoggerType::Async) {
    logger_ = std::make_shared<log::AsyncLogger>();
  } else {
    logger_ = std::make_shared<log::SyncLogger>();
  }

  logger_->Init(conf_.get_log_level(), conf_.get_log_filename());
}

void Server::setup_ssl_context(boost::beast::error_code &ec) {
  ssl_ctx_ = std::make_shared<boost::asio::ssl::context>(
      boost::asio::ssl::context::tls_server);
  ssl_ctx_->set_options(boost::asio::ssl::context::default_workarounds |
                        boost::asio::ssl::context::no_sslv2 |
                        boost::asio::ssl::context::no_sslv3 |
                        boost::asio::ssl::context::no_tlsv1 |
                        boost::asio::ssl::context::no_tlsv1_1 |
                        boost::asio::ssl::context::single_dh_use);

  ssl_ctx_->use_certificate_chain_file(conf_.get_cert_path(), ec);
  if (ec) {
    return;
  }
  ssl_ctx_->use_private_key_file(conf_.get_pri_key_path(),
                                 boost::asio::ssl::context::file_format::pem,
                                 ec);
}

void Server::listen(boost::asio::io_context &ioc,
                    const boost::asio::ip::tcp::endpoint &ep,
                    const boost::asio::yield_context &yield) {
  boost::beast::error_code ec;

  boost::asio::ip::tcp::acceptor acceptor{ioc};
  acceptor.open(ep.protocol(), ec);
  if (ec) {
    logger_->Error(
        boost::format("acceptor failed to open socket, detail: [%1%]") %
        ec.message());
    return;
  }
  acceptor.set_option(boost::asio::socket_base::reuse_address(true), ec);
  if (ec) {
    logger_->Error(
        boost::format("acceptor failed to set socket option, detail: [%1%]") %
        ec.message());
    return;
  }
  acceptor.bind(ep, ec);
  if (ec) {
    logger_->Error(
        boost::format("acceptor failed to bind address, detail: [%1%]") %
        ec.message());
    return;
  }
  acceptor.listen(boost::asio::socket_base::max_listen_connections, ec);
  if (ec) {
    logger_->Error(
        boost::format("acceptor failed to listen connection, detail: [%1%]") %
        ec.message());
    return;
  }

  while (true) {
    boost::asio::ip::tcp::socket socket{ioc};
    acceptor.async_accept(socket, yield[ec]);
    if (ec) {
      logger_->Error(
          boost::format("acceptor failed to accept connection, detail: [%1%]") %
          ec.message());
      continue;
    }

    std::chrono::seconds timeout{conf_.get_timeout()};
    auto session = std::make_shared<Session>(
        std::move(socket), timeout, router_, ssl_ctx_,
        conf_.get_request_header_limit(), conf_.get_request_body_limit());
    boost::asio::spawn(acceptor.get_executor(),
                       std::bind(&Session::handle_session, session, logger_,
                                 std::placeholders::_1));
  }
}

}  // namespace boat