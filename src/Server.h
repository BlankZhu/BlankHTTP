#pragma once

#include <fstream>
#include <functional>
#include <string>
#include <thread>
#include <unordered_map>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/thread.hpp>

#include "AsyncLogger.h"
#include "Config.h"
#include "HandleChain.h"
#include "Handler.h"
#include "Router.h"
#include "Session.h"
#include "SyncLogger.h"

namespace blank {
namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

class Server {
 public:
  Server(Config &conf)
      : conf_(conf),
        router_(std::make_shared<Router>()),
        ssl_ctx_(nullptr),
        initialized_(false) {}
  ~Server() = default;

 public:
  void init(LoggerType type = LoggerType::Async);
  void run();
  void run(net::io_context &ioc);
  void register_handler(const std::string &path, const http::verb &method,
                        HandlerPtr handler,
                        bool enable_default_middlewares = true);
  void register_chain(const std::string &path, const http::verb &method,
                      HandleChainPtr handle_chain);

 private:
  void setup_logger(LoggerType type);
  void setup_ssl_context(beast::error_code &ec);
  void listen(net::io_context &ioc, tcp::endpoint ep, net::yield_context yield);

 private:
  Config conf_;
  RouterPtr router_;
  LoggerInterfacePtr logger_;
  std::shared_ptr<ssl::context> ssl_ctx_;
  bool initialized_;
};
}  // namespace blank