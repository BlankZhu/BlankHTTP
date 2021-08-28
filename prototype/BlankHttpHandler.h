#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include "BlankHttpContext.h"
#include "BlankHttpConstant.h"

namespace blank
{
    namespace http = boost::beast::http;

    using Response = http::response<http::string_body>;
    using Request = http::request<http::string_body>;

    class BlankHttpHandler
    {
    public:
        BlankHttpHandler() = default;
        virtual ~BlankHttpHandler() = default;

    public:
        virtual Response handle_request(BlankHttpContextPtr ctx, Request &&req);
        virtual Response handle_delete(BlankHttpContextPtr ctx, Request &&req);
        virtual Response handle_get(BlankHttpContextPtr ctx, Request &&req);
        virtual Response handle_head(BlankHttpContextPtr ctx, Request &&req);
        virtual Response handle_post(BlankHttpContextPtr ctx, Request &&req);
        virtual Response handle_put(BlankHttpContextPtr ctx, Request &&req);
        virtual Response handle_connect(BlankHttpContextPtr ctx, Request &&req);
        virtual Response handle_options(BlankHttpContextPtr ctx, Request &&req);
        virtual Response handle_trace(BlankHttpContextPtr ctx, Request &&req);

    private:
        Response base_not_found(BlankHttpContextPtr ctx, Request &&req);
    };

    using BlankHttpHandlerPtr = std::shared_ptr<BlankHttpHandler>;
};
