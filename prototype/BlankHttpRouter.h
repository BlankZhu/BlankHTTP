#pragma once

#include <memory>
#include <shared_mutex>

#include "BlankHttpContext.h"
#include "BlankHttpHandler.h"
#include "BlankHttpHandleChain.h"
#include "BlankHttpRouteTable.h"

namespace blank
{
    class BlankHttpRouter
    {
    public:
        BlankHttpRouter() = default;
        ~BlankHttpRouter() = default;

    public:
        void add_handler(const std::string &path, BlankHttpHandlerPtr handler);
        BlankHttpHandlerPtr get_handler(BlankHttpContextPtr context) const;

    private:
        BlankHttpRouteTable table_;
        mutable std::shared_mutex mutex_;
    };

    using BlankHttpRouterPtr = std::shared_ptr<BlankHttpRouter>;
};