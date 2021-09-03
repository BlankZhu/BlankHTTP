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

    class BlankHttpClient
    {
    public:
        BlankHttpClient(const std::string &host, const std::string &port,
                        const std::chrono::seconds &timeout, net::any_io_executor executor)
            : host_(host), port_(port), timeout_(timeout), is_connected_(false),
              resolver_(executor), stream_(executor) {}

        BlankHttpClient(BlankHttpClient &&) = default;

        ~BlankHttpClient()
        {
            if (is_connected_)
            {
                beast::error_code ec;
                Shutdown(ec);
            }
        }

        void Connect(net::yield_context &yield, beast::error_code &ec)
        {
            if (is_connected_)
            {
                return;
            }

            stream_.expires_after(timeout_);
            auto const results = resolver_.async_resolve(host_, port_, yield[ec]);
            if (ec)
            {
                return;
            }

            stream_.async_connect(results, yield[ec]);
            if (ec)
            {
                return;
            }

            is_connected_ = true;
        }

        void Shutdown(beast::error_code &ec)
        {
            if (is_connected_)
            {
                stream_.expires_after(timeout_);
                stream_.socket().shutdown(tcp::socket::shutdown_both, ec);
                is_connected_ = false;
            }
        }

        template <class Body, class Fields>
        void DoRequest(const http::message<true, Body, Fields> &request,
                       http::message<false, Body, Fields> &response,
                       net::yield_context &yield, beast::error_code &ec)
        {
            if (!is_connected_)
            {
                Connect(yield, ec);
                if (ec)
                {
                    return;
                }
            }

            stream_.expires_after(timeout_);
            http::async_write(stream_, request, yield[ec]);
            if (ec)
            {
                return;
            }

            beast::flat_buffer buffer;
            http::async_read(stream_, buffer, response, yield[ec]);
            if (ec)
            {
                return;
            }
        }

        template <class Body, class Fields>
        void Download(const http::message<true, Body, Fields> &request,
                      const std::string &save_path,
                      net::yield_context &yield, beast::error_code &ec)
        {
            if (!is_connected_)
            {
                Connect(yield, ec);
                if (ec)
                {
                    return;
                }
            }

            http::response<http::file_body> response;
            response.body().open(save_path.c_str(), beast::file_mode::write, ec);
            if (ec)
            {
                return;
            }

            stream_.expires_after(timeout_);
            http::async_write(stream_, request, yield[ec]);
            if (ec)
            {
                return;
            }

            beast::flat_buffer buffer;
            http::async_read(stream_, buffer, response, yield[ec]);
            if (ec)
            {
                return;
            }
        }

    private:
        std::string host_;
        std::string port_;
        std::chrono::seconds timeout_;
        bool is_connected_;
        tcp::resolver resolver_;
        beast::tcp_stream stream_;
    };
}