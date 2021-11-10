#pragma once

#include <memory>

#include "Handler.h"
#include "Response.h"

namespace blank {
class Middleware;
using MiddlewarePtr = std::shared_ptr<Middleware>;

class Middleware : public Handler {
 public:
  Middleware() : Handler(), next_(nullptr){};
  virtual ~Middleware() = default;

 public:
  virtual Response handle_request(ContextPtr ctx, Request &&req);
  void set_next(HandlerPtr next);

 protected:
  Response next(ContextPtr ctx, Request &&req);

 private:
  HandlerPtr next_;
};
}  // namespace blank