#include <boat/core/Handler.h>
#include <boat/Constant.h>

#include <utility>

namespace boat {

Response Handler::handle_request(const ContextPtr ctx, Request &&req) {
  return base_not_found(ctx, std::move(req));
}

Response Handler::base_not_found(const ContextPtr& ctx, Request &&req) {
  StringResponse resp{boost::beast::http::status::not_found, req.version()};
  resp.set(boost::beast::http::field::server, constant::k_server_name);
  resp.set(boost::beast::http::field::content_type, constant::k_mime_txt);
  resp.keep_alive(req.keep_alive());
  resp.body() = "You've reached a blank space.";

  Response response{std::move(resp)};
  return response;
}

}  // namespace blank