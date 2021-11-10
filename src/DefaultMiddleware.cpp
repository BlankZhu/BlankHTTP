#include "DefaultMiddleware.h"

namespace blank {
Response DefaultMiddleware::handle_request(ContextPtr ctx, Request &&req) {
  auto start = std::chrono::system_clock::now();
  auto resp = next(ctx, std::move(req));
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<float> upstream_duration = end - start;

  ctx->logger->info(fmt("[%1%] [%2%] [%3%] [%4%]") % resp.get_status_code() %
                    req.method_string() % req.target() %
                    upstream_duration.count());

  return resp;
}
}  // namespace blank