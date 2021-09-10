#include "Router.h"

namespace blank
{
    void Router::add_handler(const std::string& path, const http::verb &method, HandlerPtr handler)
    {
        std::unique_lock lock{mutex_};
        table_.add_handler(path, method, handler);
    }

    HandlerPtr Router::get_handler(ContextPtr context) const
    {
        std::shared_lock lock{mutex_};
        auto ret = table_.get_handler(context);
        if (ret != nullptr)
        {
            return ret;
        }
        return std::make_shared<HandleChain>();
    }
};
