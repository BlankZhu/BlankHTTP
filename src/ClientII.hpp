#pragma once

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>

namespace blank {
namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

class ClientII {
 public:
  ClientII(const std::string &host, std::uint_least16_t port,
           const std::chrono::seconds &timeout,
           std::shared_ptr<ssl::context> ssl_context,
           net::any_io_executor executor)
      : host_(host),
        port_(std::to_string(port)),
        timeout_(timeout),
        ssl_context_(ssl_context),
        resolver_(executor),
        is_connected_(false) {
    if (ssl_context_ != nullptr) {
      ssl_stream_.emplace(executor, *ssl_context_);
    } else {
      tcp_stream_.emplace(executor);
    }
  }
  ClientII(ClientII &&) = default;
  ~ClientII() {
    if (is_connected_) {
      beast::error_code ec;
      Shutdown(ec);
    }
  }

 public:
  void Connect(net::yield_context yield, beast::error_code &ec) {
    if (is_connected_) {
      return;
    }

    if (tcp_stream_.has_value()) {
      connect(yield, ec);
    }
    if (ssl_stream_.has_value()) {
      connect_ssl(yield, ec);
    }
    is_connected_ = true;
  }

  void Shutdown(beast::error_code &ec) {
    if (!is_connected_) {
      return;
    }

    if (tcp_stream_.has_value()) {
      shutdown(ec);
    }
    if (ssl_stream_.has_value()) {
      shutdown_ssl(ec);
    }
    is_connected_ = false;
  }

  template <class RequestBody, class ResponseBody>
  void Do(const http::request<RequestBody> &req,
          http::response<ResponseBody> &resp, net::yield_context yield,
          beast::error_code &ec) {
    if (!is_connected_) {
      Connect(yield, ec);
      if (ec) {
        return;
      }
    }

    if (tcp_stream_.has_value()) {
      return send_and_recv(req, resp, yield, ec);
    }
    if (ssl_stream_.has_value()) {
      return send_and_recv_ssl(req, resp, yield, ec);
    }
  }

 private:
  void connect(net::yield_context yield, beast::error_code &ec) {
    auto const results = resolver_.async_resolve(host_, port_, yield[ec]);
    if (ec) {
      return;
    }

    tcp_stream_->expires_after(timeout_);
    tcp_stream_->async_connect(results, yield[ec]);
    if (ec) {
      return;
    }
  }
  void connect_ssl(net::yield_context yield, beast::error_code &ec) {
    // setup SNI hostname
    if (!SSL_set_tlsext_host_name(ssl_stream_->native_handle(),
                                  host_.c_str())) {
      ec.assign(static_cast<int>(::ERR_get_error()),
                net::error::get_ssl_category());
      return;
    }

    auto const results = resolver_.async_resolve(host_, port_, yield[ec]);
    if (ec) {
      return;
    }

    beast::get_lowest_layer(*ssl_stream_).expires_after(timeout_);
    beast::get_lowest_layer(*ssl_stream_).async_connect(results, yield[ec]);
    if (ec) {
      return;
    }

    ssl_stream_->async_handshake(ssl::stream_base::client, yield[ec]);
    if (ec) {
      return;
    }
  }

  void shutdown(beast::error_code &ec) {
    tcp_stream_->expires_after(timeout_);
    tcp_stream_->socket().shutdown(tcp::socket::shutdown_both, ec);
    return;
  }
  void shutdown_ssl(beast::error_code &ec) {
    beast::get_lowest_layer(*ssl_stream_).expires_after(timeout_);
    beast::get_lowest_layer(*ssl_stream_)
        .socket()
        .shutdown(tcp::socket::shutdown_both, ec);
    return;
  }

  template <class RequestBody, class ResponseBody>
  void send_and_recv(const http::request<RequestBody> &req,
                     http::response<ResponseBody> &resp,
                     net::yield_context yield, beast::error_code &ec) {
    tcp_stream_->expires_after(timeout_);
    http::async_write(*tcp_stream_, req, yield[ec]);
    if (ec) {
      return;
    }
    beast::flat_buffer buffer;
    http::async_read(*tcp_stream_, buffer, resp, yield[ec]);
    return;
  }
  template <class RequestBody, class ResponseBody>
  void send_and_recv_ssl(const http::request<RequestBody> &req,
                         http::response<ResponseBody> &resp,
                         net::yield_context yield, beast::error_code &ec) {
    beast::get_lowest_layer(*ssl_stream_).expires_after(timeout_);
    http::async_write(*ssl_stream_, req, yield[ec]);
    if (ec) {
      return;
    }
    beast::flat_buffer buffer;
    http::async_read(*ssl_stream_, buffer, resp, yield[ec]);
    return;
  }

 private:
  const std::string host_;
  const std::string port_;
  const std::chrono::seconds timeout_;
  tcp::resolver resolver_;
  bool is_connected_;
  std::shared_ptr<ssl::context> ssl_context_;
  boost::optional<beast::tcp_stream> tcp_stream_;
  boost::optional<beast::ssl_stream<beast::tcp_stream>> ssl_stream_;
};

}  // namespace blank