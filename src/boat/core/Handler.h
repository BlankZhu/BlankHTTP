#ifndef BOAT_CORE_HANDLER_H
#define BOAT_CORE_HANDLER_H

#include <boat/core/Context.h>
#include <boat/core/Response.h>
#include <boat/core/Request.h>

namespace boat {

class Handler {
 public:
  Handler() = default;
  virtual ~Handler() = default;

  virtual Response handle_request(ContextPtr ctx, Request &&req);

 private:
  static Response base_not_found(const ContextPtr& ctx, Request &&req);
};

using HandlerPtr = std::shared_ptr<Handler>;
}  // namespace blank

#endif