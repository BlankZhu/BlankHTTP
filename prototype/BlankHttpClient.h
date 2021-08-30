#pragma once

#include <chrono>
#include <string>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace blank
{
    namespace beast = boost::beast;
    namespace http = boost::beast::http;
    namespace net = boost::asio;
    using tcp = boost::asio::ip::tcp;
    using Request = http::request<http::string_body>;
    using Response = http::response<http::string_body>;

    class BlankHttpClient
    {
    public:
        BlankHttpClient(const std::string &host, const std::string &port,
                        const std::chrono::seconds &timeout, net::any_io_executor executor)
            : host_(host), port_(port), timeout_(timeout), is_connected_(false),
              resolver_(executor), stream_(executor) {}
        BlankHttpClient(BlankHttpClient &&) = default;
        ~BlankHttpClient();

        void Connect(net::yield_context &yield, beast::error_code &ec);
        void Shutdown(beast::error_code &ec);
        void DoRequest(const Request &request, Response &response,
                       net::yield_context &yield, beast::error_code &ec);
        void Download(const Request &request, const std::string &save_path,
                      net::yield_context &yield, beast::error_code &ec);

    private:
        std::string host_;
        std::string port_;
        std::chrono::seconds timeout_;
        bool is_connected_;
        tcp::resolver resolver_;
        beast::tcp_stream stream_;
    };
}