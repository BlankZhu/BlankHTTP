#pragma once

#include <chrono>
#include <memory>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include "BlankHttpContext.h"
#include "BlankHttpLogger.h"
#include "BlankHttpRequestTarget.h"
#include "BlankHttpRouter.h"

namespace blank
{
    namespace beast = boost::beast;
    namespace http = boost::beast::http;
    namespace net = boost::asio;
    using tcp = boost::asio::ip::tcp;

    using Request = http::request<http::string_body>;

    class BlankHttpSession : public std::enable_shared_from_this<BlankHttpSession>
    {
    public:
        BlankHttpSession(tcp::socket &&socket,
                         const std::chrono::seconds timeout,
                         const BlankHttpRouterPtr router)
            : stream_(std::move(socket)), router_(router), timeout_(timeout) {}
        ~BlankHttpSession() = default;

    public:
        void handle_session(net::yield_context yield);

    private:
        bool write_string_response(BlankHttpResponse resp, net::yield_context &yield, beast::error_code &ec);
        bool write_file_response(BlankHttpResponse resp, net::yield_context &yield, beast::error_code &ec);

    private:
        beast::tcp_stream stream_;
        const BlankHttpRouterPtr router_;
        std::chrono::seconds timeout_;
        beast::flat_buffer buffer_;
    };
}