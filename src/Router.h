#pragma once

#include <memory>
#include <shared_mutex>

#include <boost/beast/http.hpp>

#include "Context.h"
#include "HandleChain.h"
#include "Handler.h"
#include "RouteTable.h"

namespace blank
{
    namespace http = boost::beast::http;

    class Router
    {
    public:
        Router() = default;
        ~Router() = default;

    public:
        void add_handler(const std::string &path, const http::verb &method, HandlerPtr handler);
        HandlerPtr get_handler(ContextPtr context) const;

    private:
        RouteTable table_;
        mutable std::shared_mutex mutex_;
    };

    using RouterPtr = std::shared_ptr<Router>;
};