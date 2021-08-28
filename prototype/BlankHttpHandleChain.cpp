#include "BlankHttpHandleChain.h"

namespace blank
{
    BlankHttpHandleChain::BlankHttpHandleChain() : BlankHttpHandleChain(std::make_shared<BlankHttpHandler>(), true)
    {
    }

    BlankHttpHandleChain::BlankHttpHandleChain(BlankHttpHandlerPtr handler, bool use_default)
        : BlankHttpHandler()
    {
        handler_ = handler;
        if (use_default)
        {
            auto default_middleware = std::make_shared<BlankHttpDefaultMiddleware>();
            default_middleware->set_next(handler_);
            middlewares_.push_back(default_middleware);
        }
    }

    BlankHttpHandleChain::BlankHttpHandleChain(
        std::vector<BlankHttpMiddlewarePtr> middlewares,
        BlankHttpHandlerPtr handler,
        bool use_default)
        : BlankHttpHandler()
    {
        if (use_default)
        {
            auto default_middleware = std::make_shared<BlankHttpDefaultMiddleware>();
            middlewares_.push_back(default_middleware);
        }

        middlewares_.reserve(middlewares_.size() + 1);
        middlewares_.insert(middlewares_.end(), middlewares.begin(), middlewares.end());
        handler_ = handler;

        // chain middlewares together
        auto p1 = middlewares_.begin() + 1;
        auto p2 = middlewares_.begin();
        while (p1 != middlewares_.end() && p2 != middlewares_.end()) // in case that the middlewares is empty
        {
            (*p2)->set_next(*p1);
            p1 += 1;
            p2 += 1;
        }

        // chain middlewares with final hander
        (*p2)->set_next(handler_);
    }

    Response BlankHttpHandleChain::handle_request(BlankHttpContextPtr context, Request &&request)
    {
        if (middlewares_.empty())
        {
            return handler_->handle_request(context, std::move(request));
        }
        return middlewares_.front()->handle_request(context, std::move(request));
    }
};