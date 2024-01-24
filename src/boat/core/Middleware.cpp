#include <boat/core/Middleware.h>

namespace boat {

Response Middleware::handle_request(const ContextPtr ctx, Request &&req) {
  // a base http middleware will do nothing
  return next(ctx, std::move(req));
}

void Middleware::set_next(const HandlerPtr &next) { next_ = next; }

Response Middleware::next(const ContextPtr &ctx, Request &&req) const {
  return next_->handle_request(ctx, std::move(req));
}

Response DefaultMiddleware::handle_request(const ContextPtr ctx, Request &&req) {
  const auto start = std::chrono::system_clock::now();
  auto resp = next(ctx, std::move(req));
  const auto end = std::chrono::system_clock::now();
  const std::chrono::duration<float> upstream_duration = end - start;

  ctx->logger->Info(boost::format("[%1%] [%2%] [%3%] [%4%]") %
                    resp.get_status_code() % req.method_string() %
                    req.target() % upstream_duration.count());

  return std::move(resp);
}

}  // namespace boat