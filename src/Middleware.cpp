#include "Middleware.h"

namespace blank {
Response Middleware::handle_request(ContextPtr ctx, Request &&req) {
    // a base http middleware will do nothing
    return next(ctx, std::move(req));
}

void Middleware::set_next(HandlerPtr next) { next_ = next; }

Response Middleware::next(ContextPtr ctx, Request &&req) {
    return next_->handle_request(ctx, std::move(req));
}
};  // namespace blank