#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include "Context.h"
#include "Constant.h"
#include "Response.h"

namespace blank
{
    namespace beast = boost::beast;
    namespace http = boost::beast::http;

    using Request = http::request<http::string_body>;

    class Handler
    {
    public:
        Handler() = default;
        virtual ~Handler() = default;

    public:
        virtual Response handle_request(ContextPtr ctx, Request &&req);

    private:
        Response base_not_found(ContextPtr ctx, Request &&req);
    };

    using HandlerPtr = std::shared_ptr<Handler>;
};
