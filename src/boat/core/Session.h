#pragma once

#include <boost/beast/http/serializer.hpp>
#include <boost/beast/http/write.hpp>
#include <chrono>
#include <cstdint>
#include <memory>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/optional.hpp>

#include "Constant.h"
#include "Context.h"
#include "LoggerInterface.h"
#include "RequestTarget.h"
#include "Router.h"

namespace blank {
namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

using Request = http::request<http::string_body /*, Allocator */>;
using Parser = http::request_parser<http::string_body /*, Allocator */>;

class Session {
 public:
  Session(tcp::socket &&socket, const std::chrono::seconds timeout,
          const RouterPtr router,
          const std::shared_ptr<ssl::context> ssl_context = nullptr,
          const std::uint32_t &request_header_limit =
              constant::k_default_request_header_limit,
          const std::uint64_t &request_body_limit =
              constant::k_default_request_body_limit)
      : timeout_(timeout),
        router_(router),
        ssl_context_(ssl_context),
        request_header_limit_(request_header_limit),
        request_body_limit_(request_body_limit) {
    if (ssl_context_ != nullptr) {
      ssl_stream_.emplace(std::move(socket), (*ssl_context_));
    } else {
      tcp_stream_.emplace(std::move(socket));
    }
  }
  ~Session() = default;

 public:
  void handle_session(LoggerInterfacePtr &logger, net::yield_context yield);

 private:
  void reset_expire_time();
  void perform_ssl_handshake(net::yield_context yield, beast::error_code &ec);
  ContextPtr setup_context(const Request &req, bool enable_ssl,
                           LoggerInterfacePtr logger, net::yield_context yield);
  void write_response(Response &resp, int http_version, bool enable_ssl,
                      net::yield_context yield, beast::error_code &ec);
  template <class BodyT>
  void _write_response(http::response<BodyT> &resp, int http_version,
                       bool enable_ssl, net::yield_context yield,
                       beast::error_code &ec) {
    resp.prepare_payload();
    http::response_serializer<BodyT> serializer{resp};
    reset_expire_time();

    if (enable_ssl) {
      http::async_write(*ssl_stream_, serializer, yield[ec]);
    } else {
      http::async_write(*tcp_stream_, serializer, yield[ec]);
    }
  }

 private:
  boost::optional<beast::tcp_stream> tcp_stream_;
  boost::optional<beast::ssl_stream<beast::tcp_stream>> ssl_stream_;
  const std::chrono::seconds timeout_;
  const RouterPtr router_;
  const std::shared_ptr<ssl::context> ssl_context_;
  const std::uint32_t request_header_limit_;
  const std::uint64_t request_body_limit_;
  boost::optional<Parser> parser_;
  beast::flat_buffer buffer_;
};

}  // namespace blank