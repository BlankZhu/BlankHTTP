#include "Context.h"

namespace blank {
void Context::set_param(const std::string &key, const std::string &value) {
  param_.insert_or_assign(key, value);
}

std::string Context::get_param(const std::string &key) const {
  auto found = param_.find(key);
  if (found == param_.end()) {
    return "";
  }
  return found->second;
}

std::string Context::get_path() const { return request_target_->get_path(); }

std::vector<std::string> Context::get_query(const std::string &key) const {
  return request_target_->get_query(key);
}

std::string Context::get_fragment() const {
  return request_target_->get_fragment();
}

http::verb Context::get_method() const { return request_method_; }

net::any_io_executor Context::get_executor() { return executor_; }

net::yield_context &Context::get_yield_context() { return yield_; }
};  // namespace blank