#pragma once

#include <memory>

#include "BlankHttpHandler.h"
#include "BlankHttpResponse.h"

namespace blank
{
    class BlankHttpMiddleware;
    using BlankHttpMiddlewarePtr = std::shared_ptr<BlankHttpMiddleware>;

    class BlankHttpMiddleware : public BlankHttpHandler
    {
    public:
        BlankHttpMiddleware() : BlankHttpHandler(), next_(nullptr){};
        virtual ~BlankHttpMiddleware() = default;

    public:
        virtual BlankHttpResponse handle_request(BlankHttpContextPtr ctx, Request &&req);
        void set_next(BlankHttpHandlerPtr next);

    protected:
        BlankHttpResponse next(BlankHttpContextPtr ctx, Request &&req);

    private:
        BlankHttpHandlerPtr next_;
    };
};