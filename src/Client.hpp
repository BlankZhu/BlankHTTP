#pragma once

#include <chrono>
#include <string>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace blank {
namespace beast = boost::beast;
namespace http = boost::beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

class Client {
 public:
  Client(const std::string &host, const std::string &port,
         const std::chrono::seconds &timeout, net::any_io_executor executor)
      : host_(host),
        port_(port),
        timeout_(timeout),
        is_connected_(false),
        resolver_(executor),
        stream_(executor) {}
  Client(Client &&) = default;
  ~Client();

 public:
  void Connect(net::yield_context &yield, beast::error_code &ec);
  void Shutdown(beast::error_code &ec);
  template <class Body, class Fields>
  void DoRequest(const http::message<true, Body, Fields> &request,
                 http::message<false, Body, Fields> &response,
                 net::yield_context &yield, beast::error_code &ec);
  template <class Body, class Fields>
  void Download(const http::message<true, Body, Fields> &request,
                const std::string &save_path, net::yield_context &yield,
                beast::error_code &ec);

 private:
  std::string host_;
  std::string port_;
  std::chrono::seconds timeout_;
  bool is_connected_;
  tcp::resolver resolver_;
  beast::tcp_stream stream_;
};
}  // namespace blank