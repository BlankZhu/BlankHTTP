#pragma once

#include <memory>
#include <shared_mutex>

#include <boost/beast/http.hpp>

#include "BlankHttpContext.h"
#include "BlankHttpHandler.h"
#include "BlankHttpHandleChain.h"
#include "BlankHttpRouteTable.h"

namespace blank
{
    namespace http = boost::beast::http;

    class BlankHttpRouter
    {
    public:
        BlankHttpRouter() = default;
        ~BlankHttpRouter() = default;

    public:
        void add_handler(const std::string &path, const http::verb &method, BlankHttpHandlerPtr handler);
        BlankHttpHandlerPtr get_handler(BlankHttpContextPtr context) const;

    private:
        BlankHttpRouteTable table_;
        mutable std::shared_mutex mutex_;
    };

    using BlankHttpRouterPtr = std::shared_ptr<BlankHttpRouter>;
};