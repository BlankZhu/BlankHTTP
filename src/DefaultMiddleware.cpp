#include "DefaultMiddleware.h"
#include <fmt/core.h>

namespace blank {
Response DefaultMiddleware::handle_request(ContextPtr ctx, Request &&req) {
  auto start = std::chrono::system_clock::now();
  auto resp = next(ctx, std::move(req));
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<float> upstream_duration = end - start;

  std::string_view method{req.method_string().data(),
                          req.method_string().size()};
  std::string_view target{req.target().data(), req.target().size()};

  ctx->logger->info(fmt::format("[{}] [{}] [{}] [{}]", resp.get_status_code(),
                                method, target, upstream_duration.count()));

  return resp;
}
};  // namespace blank