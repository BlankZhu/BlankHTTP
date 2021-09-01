#include "BlankHttpRouter.h"

namespace blank
{
    void BlankHttpRouter::add_handler(const std::string& path, const http::verb &method, BlankHttpHandlerPtr handler)
    {
        std::unique_lock lock{mutex_};
        table_.add_handler(path, method, handler);
    }

    BlankHttpHandlerPtr BlankHttpRouter::get_handler(BlankHttpContextPtr context) const
    {
        std::shared_lock lock{mutex_};
        auto ret = table_.get_handler(context);
        if (ret != nullptr)
        {
            return ret;
        }
        return std::make_shared<BlankHttpHandleChain>();
    }
};
