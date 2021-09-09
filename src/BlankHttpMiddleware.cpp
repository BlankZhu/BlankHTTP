#include "BlankHttpMiddleware.h"

namespace blank
{
    BlankHttpResponse BlankHttpMiddleware::handle_request(BlankHttpContextPtr ctx, Request &&req)
    {
        // a base http middleware will do nothing
        return next(ctx, std::move(req));
    }

    void BlankHttpMiddleware::set_next(BlankHttpHandlerPtr next)
    {
        next_ = next;
    }

    BlankHttpResponse BlankHttpMiddleware::next(BlankHttpContextPtr ctx, Request &&req)
    {
        return next_->handle_request(ctx, std::move(req));
    }
};