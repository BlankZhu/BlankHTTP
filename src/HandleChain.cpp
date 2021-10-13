#include "HandleChain.h"

namespace blank {
HandleChain::HandleChain() : HandleChain(std::make_shared<Handler>(), true) {}

HandleChain::HandleChain(HandlerPtr handler, bool use_default) : Handler() {
  handler_ = handler;
  if (use_default) {
    auto default_middleware = std::make_shared<DefaultMiddleware>();
    default_middleware->set_next(handler_);
    middlewares_.push_back(default_middleware);
  }
}

HandleChain::HandleChain(std::vector<MiddlewarePtr> middlewares,
                         HandlerPtr handler, bool use_default)
    : Handler() {
  if (use_default) {
    auto default_middleware = std::make_shared<DefaultMiddleware>();
    middlewares_.push_back(default_middleware);
  }

  middlewares_.reserve(middlewares_.size() + 1);
  middlewares_.insert(middlewares_.end(), middlewares.begin(),
                      middlewares.end());
  handler_ = handler;

  // chain middlewares together
  auto p1 = middlewares_.begin() + 1;
  auto p2 = middlewares_.begin();
  while (p1 != middlewares_.end() &&
         p2 != middlewares_.end())  // in case that the middlewares is empty
  {
    (*p2)->set_next(*p1);
    p1 += 1;
    p2 += 1;
  }

  // chain middlewares with final hander
  (*p2)->set_next(handler_);
}

Response HandleChain::handle_request(ContextPtr context, Request &&request) {
  if (middlewares_.empty()) {
    return handler_->handle_request(context, std::move(request));
  }
  return middlewares_.front()->handle_request(context, std::move(request));
}
};  // namespace blank