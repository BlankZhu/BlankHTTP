#include "DefaultMiddleware.h"
#include <fmt/core.h>

namespace blank {
Response DefaultMiddleware::handle_request(ContextPtr ctx, Request &&req) {
  auto start = std::chrono::system_clock::now();
  auto resp = next(ctx, std::move(req));
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<float> upstream_duration = end - start;

  ctx->logger->info(fmt::format("[{}] [{}] [{}] [{}]", resp.get_status_code(),
                                req.method_string().to_string(),
                                req.target().to_string(),
                                upstream_duration.count()));

  return resp;
}
};  // namespace blank