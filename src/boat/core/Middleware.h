#ifndef BOAT_CORE_MIDDLEWARE_H
#define BOAT_CORE_MIDDLEWARE_H

#include <memory>

#include <boat/core/Handler.h>

namespace boat {

class Middleware;

class Middleware : public Handler {
 public:
  Middleware() : Handler(), next_(nullptr){};
  ~Middleware() override = default;

  Response handle_request(ContextPtr ctx, Request &&req) override;
  void set_next(const HandlerPtr &next);

 protected:
  Response next(const ContextPtr &ctx, Request &&req) const;

 private:
  HandlerPtr next_;
};
using MiddlewarePtr = std::shared_ptr<Middleware>;

class DefaultMiddleware final : public Middleware {
 public:
  DefaultMiddleware() : Middleware() {}
  ~DefaultMiddleware() override = default;

 public:
  Response handle_request(ContextPtr ctx, Request &&req) override;
};

}  // namespace boat

#endif