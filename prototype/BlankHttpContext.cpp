#include "BlankHttpContext.h"

namespace blank
{
    void BlankHttpContext::set_param(const std::string &key, const std::string &value)
    {
        param_.insert_or_assign(key, value);
    }

    std::string BlankHttpContext::get_param(const std::string &key) const
    {
        auto found = param_.find(key);
        if (found == param_.end())
        {
            return "";
        }
        return found->second;
    }

    std::string BlankHttpContext::get_path() const
    {
        return request_target_->get_path();
    }

    std::vector<std::string> BlankHttpContext::get_query(const std::string &key) const
    {
        return request_target_->get_query(key);
    }

    std::string BlankHttpContext::get_fragment() const
    {
        return request_target_->get_fragment();
    }

    net::yield_context &BlankHttpContext::get_yield_context()
    {
        return yield_;
    }
};