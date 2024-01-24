#include <boat/core/Context.h>

namespace boat {

std::any Context::get_shared_data() const { return shared_data_; }

void Context::set_shared_data(const std::any &data) { shared_data_ = data; }

std::optional<std::string> Context::get_param(const std::string& key) const {
  const auto found = param_.find(key);
  if (found == param_.cend()) {
    return std::nullopt;
  }
  return found->second;
}

void Context::set_param(const std::string &key,
                        const std::string &value) {
  param_.insert_or_assign(key, value);
}

boost::url Context::get_url() const { return url_; }

boost::beast::http::verb Context::get_request_method() const {
  return request_method_;
}

boost::asio::any_io_executor &Context::get_executor() const {
  return executor_;
}

boost::asio::yield_context &Context::get_yield_context() const {
  return yield_;
}

}  // namespace boat