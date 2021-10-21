#include "Handler.h"

namespace blank {
Response Handler::handle_request(ContextPtr ctx, Request &&req) {
  return base_not_found(ctx, std::move(req));
}

Response Handler::base_not_found(ContextPtr ctx, Request &&req) {
  StringResponse resp{http::status::not_found, req.version()};
  resp.set(http::field::server, constant::k_server_name);
  resp.set(http::field::content_type, constant::k_mime_txt);
  resp.keep_alive(req.keep_alive());
  resp.body() = "You've reached a blank space.";

  return resp;
}
};  // namespace blank