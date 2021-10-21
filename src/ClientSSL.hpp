#pragma once

#include <chrono>
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

class ClientSSL {
 public:
  ClientSSL(const std::string &host, const std::string &port,
            const std::chrono::seconds &timeout, ssl::context &ssl_context,
            net::any_io_executor executor, net::yield_context &yield)
      : host_(host),
        port_(port),
        timeout_(timeout),
        is_connected_(false),
        ssl_context_(ssl_context),
        resolver_(executor),
        stream_(executor, ssl_context),
        yield_(yield) {}
  ClientSSL(ClientSSL &&) = default;
  ~ClientSSL() {
    if (is_connected_) {
      beast::error_code ec;
      Shutdown(ec);
    }
  }

 public:
  void Connect(beast::error_code &ec) {
    if (is_connected_) {
      return;
    }

    // setup SNI Hostname
    if (!SSL_set_tlsext_host_name(stream_.native_handle(), host_.c_str())) {
      ec.assign(static_cast<int>(::ERR_get_error()),
                net::error::get_ssl_category());
      return;
    }

    auto const endpoints = resolver_.async_resolve(host_, port_, yield_[ec]);
    if (ec) {
      return;
    }

    // set timeout
    beast::get_lowest_layer(stream_).expires_after(timeout_);
    beast::get_lowest_layer(stream_).async_connect(endpoints, yield_[ec]);
    if (ec) {
      return;
    }

    // perform ssl handshake
    stream_.async_handshake(ssl::stream_base::client, yield_[ec]);
    if (ec) {
      return;
    }

    is_connected_ = true;
  }

  void Shutdown(beast::error_code &ec) {
    if (is_connected_) {
      beast::get_lowest_layer(stream_).expires_after(timeout_);
      stream_.async_shutdown(yield_[ec]);
      if (ec == net::error::eof) {
        ec = {};
      }

      is_connected_ = false;
    }
  }

  template <class Body, class Fields>
  void DoRequest(const http::message<true, Body, Fields> &request,
                 http::message<false, Body, Fields> &response,
                 beast::error_code &ec) {
    if (!is_connected_) {
      Connect(ec);
      if (ec) {
        return;
      }
    }

    beast::get_lowest_layer(stream_).expires_after(timeout_);
    http::async_write(stream_, request, yield_[ec]);
    if (ec) {
      return;
    }

    beast::flat_buffer buffer;
    http::async_read(stream_, buffer, response, yield_[ec]);
    if (ec) {
      return;
    }
  }

  template <class Body, class Fields>
  void Download(const http::message<true, Body, Fields> &request,
                const std::string &save_path, beast::error_code &ec) {
    if (!is_connected_) {
      Connect(ec);
      if (ec) {
        return;
      }
    }

    http::response<http::file_body> response;
    response.body().open(save_path.c_str(), beast::file_mode::write, ec);
    if (ec) {
      return;
    }

    beast::get_lowest_layer(stream_).expires_after(timeout_);
    http::async_write(stream_, request, yield_[ec]);
    if (ec) {
      return;
    }

    beast::flat_buffer buffer;
    http::async_read(stream_, buffer, response, yield_[ec]);
    if (ec) {
      return;
    }
  }

 private:
  std::string host_;
  std::string port_;
  std::chrono::seconds timeout_;
  bool is_connected_;
  ssl::context &ssl_context_;
  tcp::resolver resolver_;
  beast::ssl_stream<beast::tcp_stream> stream_;
  net::yield_context yield_;
};
}  // namespace blank
