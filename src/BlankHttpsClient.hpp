#pragma once

#include <chrono>
#include <string>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>

namespace blank
{
    namespace beast = boost::beast;
    namespace http = boost::beast::http;
    namespace net = boost::asio;
    namespace ssl = boost::asio::ssl;
    using tcp = boost::asio::ip::tcp;

    class BlankHttpsClient
    {
    public:
        BlankHttpsClient(
            const std::string &host,
            const std::string &port,
            const std::chrono::seconds &timeout,
            ssl::context &ssl_context,
            net::any_io_executor executor,
            net::yield_context &yield)
            : host_(host),
              port_(port),
              timeout_(timeout),
              is_connected_(false),
              ssl_context_(ssl_context),
              resolver_(executor),
              stream_(executor, ssl_context),
              yield_(yield)
        {
        }
        BlankHttpsClient(BlankHttpsClient &&) = default;
        ~BlankHttpsClient();

    public:
        void Connect(beast::error_code &ec);
        void Shutdown(beast::error_code &ec);
        template <class Body, class Fields>
        void DoRequest(const http::message<true, Body, Fields> &request,
                       http::message<false, Body, Fields> &response,
                       beast::error_code &ec);
        template <class Body, class Fields>
        void Download(const http::message<true, Body, Fields> &request,
                      const std::string &save_path,
                      beast::error_code &ec);

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
}