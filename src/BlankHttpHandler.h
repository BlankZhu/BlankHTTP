#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include "BlankHttpContext.h"
#include "BlankHttpConstant.h"
#include "BlankHttpResponse.h"

namespace blank
{
    namespace beast = boost::beast;
    namespace http = boost::beast::http;

    using Request = http::request<http::string_body>;

    class BlankHttpHandler
    {
    public:
        BlankHttpHandler() = default;
        virtual ~BlankHttpHandler() = default;

    public:
        virtual BlankHttpResponse handle_request(BlankHttpContextPtr ctx, Request &&req);

    private:
        BlankHttpResponse base_not_found(BlankHttpContextPtr ctx, Request &&req);
    };

    using BlankHttpHandlerPtr = std::shared_ptr<BlankHttpHandler>;
};
