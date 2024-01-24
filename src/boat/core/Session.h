#ifndef BOAT_CORE_SESSION_H
#define BOAT_CORE_SESSION_H

#include <chrono>
#include <cstdint>
#include <memory>
#include <optional>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/http/serializer.hpp>
#include <boost/beast/http/write.hpp>
#include <boost/beast/ssl.hpp>

#include <boat/Constant.h>
#include <boat/core/Context.h>
#include <boat/core/Router.h>
#include <boat/log/LoggerInterface.h>

namespace boat {

using Request = boost::beast::http::request<
    boost::beast::http::string_body /*, Allocator */>;
using Parser = boost::beast::http::request_parser<
    boost::beast::http::string_body /*, Allocator */>;

class Session {
 public:
  Session(boost::asio::ip::tcp::socket &&socket,
          const std::chrono::seconds timeout, RouterPtr router,
          std::shared_ptr<boost::asio::ssl::context> ssl_context = nullptr,
          const std::uint32_t &request_header_limit =
              constant::k_default_request_header_limit,
          const std::uint64_t &request_body_limit =
              constant::k_default_request_body_limit)
      : timeout_(timeout),
        router_(std::move(router)),
        ssl_context_(std::move(ssl_context)),
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
  void handle_session(const log::LoggerInterfacePtr &logger,
                      const boost::asio::yield_context &yield);

 private:
  void reset_expire_time();
  void perform_ssl_handshake(const boost::asio::yield_context &yield,
                             boost::beast::error_code &ec);
  ContextPtr setup_context(const Request &req, bool enable_ssl,
                           log::LoggerInterfacePtr logger,
                           boost::asio::yield_context yield);
  void write_response(Response &resp, unsigned http_version, bool enable_ssl,
                      const boost::asio::yield_context &yield,
                      boost::beast::error_code &ec);
  template <class BodyT>
  void _write_response(boost::beast::http::response<BodyT> &resp,
                       const unsigned http_version, const bool enable_ssl,
                       const boost::asio::yield_context &yield,
                       boost::beast::error_code &ec) {
    resp.prepare_payload();
    boost::beast::http::response_serializer<BodyT> serializer{resp};
    reset_expire_time();

    if (enable_ssl) {
      boost::beast::http::async_write(*ssl_stream_, serializer, yield[ec]);
    } else {
      boost::beast::http::async_write(*tcp_stream_, serializer, yield[ec]);
    }
  }

  std::optional<boost::beast::tcp_stream> tcp_stream_;
  std::optional<boost::beast::ssl_stream<boost::beast::tcp_stream>> ssl_stream_;
  const std::chrono::seconds timeout_;
  const RouterPtr router_;
  const std::shared_ptr<boost::asio::ssl::context> ssl_context_;
  const std::uint32_t request_header_limit_;
  const std::uint64_t request_body_limit_;
  boost::optional<Parser> parser_;
  boost::beast::flat_buffer buffer_;
};

}  // namespace boat

#endif