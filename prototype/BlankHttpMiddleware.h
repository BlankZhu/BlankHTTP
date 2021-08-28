#pragma once

#include <memory>

#include "BlankHttpHandler.h"

namespace blank
{
    class BlankHttpMiddleware;
    using BlankHttpMiddlewarePtr = std::shared_ptr<BlankHttpMiddleware>;

    class BlankHttpMiddleware : public BlankHttpHandler
    {
    public:
        BlankHttpMiddleware() : BlankHttpHandler(), next_(nullptr) {};
        virtual ~BlankHttpMiddleware() = default;

    public:
        virtual Response handle_request(BlankHttpContextPtr ctx, Request &&req);
        void set_next(BlankHttpHandlerPtr next);

    protected:
        Response next(BlankHttpContextPtr ctx, Request &&req);

    private:
        BlankHttpHandlerPtr next_;
    };
};