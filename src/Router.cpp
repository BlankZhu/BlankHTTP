#include "Router.h"

namespace blank {
void Router::add_handler(const std::string &path, const http::verb &method,
                         HandlerPtr handler) {
    table_.add_handler(path, method, handler);
}

HandlerPtr Router::get_handler(ContextPtr context) const {
    auto ret = table_.get_handler(context);
    if (ret != nullptr) {
        return ret;
    }
    return std::make_shared<HandleChain>();
}
};  // namespace blank
