#pragma once

#include <any>
#include <memory>
#include <string>
#include <unordered_map>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/http.hpp>

#include "LoggerInterface.h"
#include "RequestTarget.h"

namespace blank {
namespace net = boost::asio;
namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;
using Param = std::unordered_map<std::string, std::string>;

class Context {
 public:
  Context(tcp::endpoint endpoint, RequestTargetPtr target,
          const http::verb &method, net::any_io_executor executor,
          net::yield_context &yield, LoggerInterfacePtr &logger)
      : remote_endpoint_(endpoint),
        request_target_(target),
        request_method_(method),
        executor_(executor),
        yield_(yield),
        logger(logger) {}
  ~Context() = default;

 public:
  std::any get_shared_data() const;
  void set_shared_data(const std::any &data);
  std::string get_param(const std::string &key) const;
  void set_param(const std::string &key, const std::string &value);
  std::string get_path() const;
  std::string get_query() const;
  std::vector<std::string> get_query(const std::string &key) const;
  std::string get_fragment() const;
  http::verb get_method() const;
  net::any_io_executor get_executor();
  net::yield_context &get_yield_context();

 private:
  std::any shared_data_;
  Param param_;
  tcp::endpoint remote_endpoint_;
  RequestTargetPtr request_target_;
  http::verb request_method_;
  net::any_io_executor executor_;
  net::yield_context &yield_;

 public:
  LoggerInterfacePtr &logger;
};

using ContextPtr = std::shared_ptr<Context>;
};  // namespace blank