#ifndef BOAT_CSERVER_H
#define BOAT_CSERVER_H

#include <string>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>

#include <boat/core/HandleChain.h>
#include <boat/core/Handler.h>
#include <boat/core/Session.h>

#include <boat/Config.h>
#include <boat/Server.h>

namespace boat {

class Server {
 public:
  explicit Server(const Config &conf)
      : conf_(conf),
        router_(std::make_shared<Router>()),
        ssl_ctx_(nullptr),
        initialized_(false) {}
  ~Server() = default;

  void init(log::LoggerType type = log::LoggerType::Async);
  void run();
  void run(boost::asio::io_context &ioc);
  void register_handler(const std::string &path,
                        const boost::beast::http::verb &method,
                        const HandlerPtr& handler,
                        bool enable_default_middlewares = true) const;
  void register_chain(const std::string &path,
                      const boost::beast::http::verb &method,
                      const HandleChainPtr& handle_chain) const;

 private:
  void setup_logger(log::LoggerType type);
  void setup_ssl_context(boost::beast::error_code &ec);
  void listen(boost::asio::io_context &ioc, const boost::asio::ip::tcp::endpoint& ep,
              const boost::asio::yield_context& yield);

  Config conf_;
  RouterPtr router_;
  log::LoggerInterfacePtr logger_;
  std::shared_ptr<boost::asio::ssl::context> ssl_ctx_;
  bool initialized_;
};

}  // namespace boat

#endif