#pragma once

#include <chrono>
#include <memory>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include "Context.h"
#include "Logger.h"
#include "RequestTarget.h"
#include "Router.h"

namespace blank
{
    namespace beast = boost::beast;
    namespace http = boost::beast::http;
    namespace net = boost::asio;
    using tcp = boost::asio::ip::tcp;

    using Request = http::request<http::string_body>;

    class Session : public std::enable_shared_from_this<Session>
    {
    public:
        Session(tcp::socket &&socket,
                         const std::chrono::seconds timeout,
                         const RouterPtr router)
            : stream_(std::move(socket)), router_(router), timeout_(timeout) {}
        ~Session() = default;

    public:
        void handle_session(net::yield_context yield);

    private:
        bool write_string_response(Response resp, net::yield_context &yield, beast::error_code &ec);
        bool write_file_response(Response resp, net::yield_context &yield, beast::error_code &ec);

    private:
        beast::tcp_stream stream_;
        const RouterPtr router_;
        std::chrono::seconds timeout_;
        beast::flat_buffer buffer_;
    };
}