#pragma once

#include <chrono>
#include <memory>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>

#include "Context.h"
#include "Logger.h"
#include "RequestTarget.h"
#include "Router.h"

namespace blank {
namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

using Request = http::request<http::string_body>;
using StringSerializer = http::response_serializer<http::string_body>;
using FileSerializer = http::response_serializer<http::file_body>;

class SessionSSL {
   public:
    SessionSSL(tcp::socket &&socket, ssl::context &ssl_ctx,
               const std::chrono::seconds timeout, const RouterPtr router)
        : stream_(std::move(socket), ssl_ctx),
          router_(router),
          timeout_(timeout) {}
    ~SessionSSL() = default;

   public:
    void handle_session(Logger &logger, net::yield_context yield);

   private:
    bool write_response(Response &&resp, int http_version,
                        net::yield_context &yield, beast::error_code &ec);
    void reset_serializer();

   private:
    beast::ssl_stream<beast::tcp_stream> stream_;
    const RouterPtr router_;
    std::chrono::seconds timeout_;
    beast::flat_buffer buffer_;  // TODO: to static buffer maybe
    boost::optional<StringSerializer> string_serializer_;
    boost::optional<FileSerializer> file_serializer_;
};
}  // namespace blank