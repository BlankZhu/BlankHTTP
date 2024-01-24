#ifndef BOAT_CLIENT_H
#define BOAT_CLIENT_H

#include <cstdint>

#include <chrono>
#include <memory>
#include <string>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <utility>

namespace boat {

class Client : public std::enable_shared_from_this<Client> {
 public:
  Client(const boost::asio::any_io_executor &executor, std::string host,
         const std::uint_least16_t port,
         const std::chrono::milliseconds &timeout,
         const std::shared_ptr<boost::asio::ssl::context> &ssl_context)
      : host_(std::move(host)),
        port_(std::to_string(port)),
        timeout_(timeout),
        resolver_(executor),
        is_connected_(false) {
    if (ssl_context != nullptr) {
      ssl_stream_.emplace(executor, *ssl_context_);
      return;
    }
    tcp_stream_.emplace(executor);
  }

  Client(Client &&)  noexcept = default;
  ~Client() {
    if (is_connected_) {
      boost::beast::error_code ec;
      Shutdown(ec);
    }
  }

 public:
  void Connect(const boost::asio::yield_context &yield,
               boost::beast::error_code &ec) {
    if (is_connected_) {
      return;
    }

    if (tcp_stream_.has_value()) {
      connect(yield, shared_from_this(), ec);
    }
    if (ssl_stream_.has_value()) {
      connect_ssl(yield, shared_from_this(), ec);
    }
    is_connected_ = true;
  }

  void Shutdown(boost::beast::error_code &ec) {
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
  void Do(const boost::beast::http::request<RequestBody> &req,
          boost::beast::http::response<ResponseBody> &resp,
          boost::asio::yield_context yield, boost::beast::error_code &ec) {
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
  static void connect(const boost::asio::yield_context &yield,
                      const std::shared_ptr<Client> &self,
                      boost::beast::error_code &ec) {
    auto const results =
        self->resolver_.async_resolve(self->host_, self->port_, yield[ec]);
    if (ec) {
      return;
    }

    self->tcp_stream_->expires_after(self->timeout_);
    self->tcp_stream_->async_connect(results, yield[ec]);
  }

  static void connect_ssl(const boost::asio::yield_context &yield,
                          const std::shared_ptr<Client> &self,
                          boost::beast::error_code &ec) {
    // setup SNI hostname
    if (!SSL_set_tlsext_host_name(self->ssl_stream_->native_handle(),
                                  self->host_.c_str())) {
      ec.assign(static_cast<int>(::ERR_get_error()),
                boost::asio::error::get_ssl_category());
      return;
    }

    auto const results =
        self->resolver_.async_resolve(self->host_, self->port_, yield[ec]);
    if (ec) {
      return;
    }

    boost::beast::get_lowest_layer(*self->ssl_stream_)
        .expires_after(self->timeout_);
    boost::beast::get_lowest_layer(*self->ssl_stream_)
        .async_connect(results, yield[ec]);
    if (ec) {
      return;
    }

    self->ssl_stream_->async_handshake(boost::asio::ssl::stream_base::client,
                                       yield[ec]);
  }

  void shutdown(boost::beast::error_code &ec) {
    tcp_stream_->expires_after(timeout_);
    tcp_stream_->socket().shutdown(boost::asio::ip::tcp::socket::shutdown_both,
                                   ec);
  }

  void shutdown_ssl(boost::beast::error_code &ec) {
    boost::beast::get_lowest_layer(*ssl_stream_).expires_after(timeout_);
    boost::beast::get_lowest_layer(*ssl_stream_)
        .socket()
        .shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
  }

  template <class RequestBody, class ResponseBody>
  void send_and_recv(const boost::beast::http::request<RequestBody> &req,
                     boost::beast::http::response<ResponseBody> &resp,
                     const boost::asio::yield_context& yield,
                     boost::beast::error_code &ec) {
    tcp_stream_->expires_after(timeout_);
    boost::beast::http::async_write(*tcp_stream_, req, yield[ec]);
    if (ec) {
      return;
    }
    boost::beast::flat_buffer buffer;
    boost::beast::http::async_read(*tcp_stream_, buffer, resp, yield[ec]);
  }

  template <class RequestBody, class ResponseBody>
  void send_and_recv_ssl(const boost::beast::http::request<RequestBody> &req,
                         boost::beast::http::response<ResponseBody> &resp,
                         const boost::asio::yield_context& yield,
                         boost::beast::error_code &ec) {
    boost::beast::get_lowest_layer(*ssl_stream_).expires_after(timeout_);
    boost::beast::http::async_write(*ssl_stream_, req, yield[ec]);
    if (ec) {
      return;
    }
    boost::beast::flat_buffer buffer;
    boost::beast::http::async_read(*ssl_stream_, buffer, resp, yield[ec]);
  }

  const std::string host_{};
  const std::string port_{};
  const std::chrono::milliseconds timeout_{};
  boost::asio::ip::tcp::resolver resolver_;
  bool is_connected_;
  std::shared_ptr<boost::asio::ssl::context> ssl_context_{nullptr};
  std::optional<boost::beast::tcp_stream> tcp_stream_{std::nullopt};
  std::optional<boost::beast::ssl_stream<boost::beast::tcp_stream>> ssl_stream_{
      std::nullopt};
};

}  // namespace boat

#endif