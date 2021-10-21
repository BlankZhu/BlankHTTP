#include "Server.h"
#include <boost/asio/io_context.hpp>
#include <functional>

namespace blank {
void Server::init(LoggerType type) {
  setup_logger(type);
  initialized_ = true;
}

void Server::run() {
  net::io_context ioc{static_cast<int>(conf_.get_threads())};
  run(ioc);
}

void Server::run(net::io_context &ioc) {
  logger_->info(fmt("running BlankHTTPServer with config: %1%") %
                conf_.detail_in_json());

  if (conf_.get_enable_ssl()) {
    beast::error_code ec;
    setup_ssl_context(ec);
    if (ec) {
      logger_->fatal(fmt("failed to setup SSL, detail: [%1%]") % ec.message());
      return;
    }
  }

  auto addr = net::ip::make_address(conf_.get_address());
  auto ep = tcp::endpoint{addr, conf_.get_port()};

  net::spawn(ioc, std::bind(&Server::listen, this, std::ref(ioc), ep,
                            std::placeholders::_1));

  std::vector<std::thread> threads;
  std::function<void(net::io_context &)> ioc_run_func;
  std::function<void(net::io_context &)> ioc_block = [](net::io_context &ioc) {
    ioc.run();
  };
  std::function<void(net::io_context &)> ioc_spin = [](net::io_context &ioc) {
    while (true) {
      ioc.poll();
    }
  };

  if (conf_.get_enable_spin()) {
    ioc_run_func = ioc_spin;
  } else {
    ioc_run_func = ioc_block;
  }

  for (auto i = 0; i < conf_.get_threads() - 1; i += 1) {
    threads.emplace_back([&ioc, &ioc_run_func]() { ioc_run_func(ioc); });
  }
  ioc_run_func(ioc);
}

void Server::register_handler(const std::string &path, const http::verb &method,
                              HandlerPtr handler,
                              bool enable_default_middlewares) {
  if (initialized_) {
    logger_->warning(
        fmt("cannot register new handler once the server is initialized"));
    return;
  }

  if (enable_default_middlewares) {
    auto chain = std::make_shared<HandleChain>(handler, true);
    router_->add_handler(path, method, chain);
    return;
  }
  router_->add_handler(path, method, handler);
}

void Server::register_chain(const std::string &path, const http::verb &method,
                            HandleChainPtr chain) {
  if (initialized_) {
    logger_->warning(
        fmt("cannot register new chain once the server is initialized"));
    return;
  }

  router_->add_handler(path, method, chain);
}

void Server::setup_logger(LoggerType type) {
  if (type == LoggerType::Async) {
    logger_ = std::make_shared<AsyncLogger>();
  } else {
    logger_ = std::make_shared<SyncLogger>();
  }

  logger_->init(conf_.get_log_level(), conf_.get_log_filename());
}

void Server::setup_ssl_context(beast::error_code &ec) {
  ssl_ctx_.set_options(ssl::context::default_workarounds |
                       ssl::context::no_sslv2 | ssl::context::no_sslv3 |
                       ssl::context::no_tlsv1 | ssl::context::no_tlsv1_1 |
                       ssl::context::single_dh_use);

  ssl_ctx_.use_certificate_chain_file(conf_.get_cert_path(), ec);
  if (ec) {
    return;
  }
  ssl_ctx_.use_private_key_file(conf_.get_pri_key_path(),
                                ssl::context::file_format::pem, ec);

  return;
}

void Server::listen(net::io_context &ioc, tcp::endpoint ep,
                    net::yield_context yield) {
  beast::error_code ec;

  tcp::acceptor acceptor{ioc};
  acceptor.open(ep.protocol(), ec);
  if (ec) {
    logger_->error(fmt("acceptor failed to open socket, detail: [%1%]") %
                   ec.message());
    return;
  }
  acceptor.set_option(net::socket_base::reuse_address(true), ec);
  if (ec) {
    logger_->error(fmt("acceptor failed to set socket option, detail: [%1%]") %
                   ec.message());
    return;
  }
  acceptor.bind(ep, ec);
  if (ec) {
    logger_->error(fmt("acceptor failed to bind address, detail: [%1%]") %
                   ec.message());
    return;
  }
  acceptor.listen(net::socket_base::max_listen_connections, ec);
  if (ec) {
    logger_->error(fmt("acceptor failed to listen connection, detail: [%1%]") %
                   ec.message());
    return;
  }

  while (true) {
    tcp::socket socket{ioc};
    acceptor.async_accept(socket, yield[ec]);
    if (ec) {
      logger_->error(
          fmt("acceptor failed to accept connection, detail: [%1%]") %
          ec.message());
      continue;
    }

    std::chrono::seconds timeout{conf_.get_timeout()};

    if (conf_.get_enable_ssl()) {
      auto session = std::make_shared<SessionSSL>(
          std::move(socket), std::ref(ssl_ctx_), timeout, router_,
          conf_.get_request_header_limit(), conf_.get_request_body_limit());
      net::spawn(acceptor.get_executor(),
                 std::bind(&SessionSSL::handle_session, session, logger_,
                           std::placeholders::_1));
    } else {
      auto session = std::make_shared<Session>(
          std::move(socket), timeout, router_, conf_.get_request_header_limit(),
          conf_.get_request_body_limit());
      net::spawn(acceptor.get_executor(),
                 std::bind(&Session::handle_session, session, logger_,
                           std::placeholders::_1));
    }
  }
}
};  // namespace blank