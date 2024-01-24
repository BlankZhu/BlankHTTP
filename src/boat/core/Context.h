#ifndef BOAT_CORE_CONTEXT_H
#define BOAT_CORE_CONTEXT_H

#include <any>
#include <memory>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/http.hpp>
#include <boost/url.hpp>

#include <boat/log/LoggerInterface.h>

namespace boat {

class Context {
 public:
  Context(boost::asio::ip::tcp::endpoint endpoint, boost::url url,
          const boost::beast::http::verb &method,
          boost::asio::any_io_executor &executor,
          boost::asio::yield_context &yield,
          boat::log::LoggerInterfacePtr &logger)
      : remote_endpoint_(std::move(endpoint)),
        url_(std::move(url)),
        request_method_(method),
        executor_(executor),
        yield_(yield),
        logger(logger) {}
  ~Context() = default;

  [[nodiscard]] std::any get_shared_data() const;
  // please use shared_ptr or other easy-copy type
  void set_shared_data(const std::any &data);
  [[nodiscard]] std::optional<std::string> get_param(const std::string& key) const;
  void set_param(const std::string& key, const std::string& value);
  [[nodiscard]] boost::url get_url() const;
  [[nodiscard]] boost::beast::http::verb get_request_method() const;
  [[nodiscard]] boost::asio::any_io_executor &get_executor() const;
  [[nodiscard]] boost::asio::yield_context &get_yield_context() const;

 private:
  std::any shared_data_;
  std::unordered_map<std::string, std::string> param_;
  boost::asio::ip::tcp::endpoint remote_endpoint_;
  boost::url url_;
  boost::beast::http::verb request_method_;
  boost::asio::any_io_executor &executor_;
  boost::asio::yield_context &yield_;

 public:
  boat::log::LoggerInterfacePtr &logger;
};

using ContextPtr = std::shared_ptr<Context>;
}  // namespace boat

#endif