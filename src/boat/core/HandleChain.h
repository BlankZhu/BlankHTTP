#ifndef BOAT_CORE_HANDLECHAIN_H
#define BOAT_CORE_HANDLECHAIN_H

#include <memory>
#include <vector>

#include <boat/core/Handler.h>
#include <boat/core/Middleware.h>

namespace boat {

class HandleChain final : public Handler {
 public:
  HandleChain();
  explicit HandleChain(HandlerPtr handler, bool use_default = true);
  HandleChain(std::vector<MiddlewarePtr> middlewares, HandlerPtr handler,
              bool use_default = true);
  ~HandleChain() override = default;

  Response handle_request(ContextPtr context, Request &&request) override;

 private:
  HandlerPtr handler_;
  std::vector<MiddlewarePtr> middlewares_;
};
using HandleChainPtr = std::shared_ptr<HandleChain>;

}  // namespace boat

#endif