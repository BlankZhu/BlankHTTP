#include "Context.h"
#include "RequestTarget.h"

namespace blank {
std::any Context::get_shared_data() const { return shared_data_; }

void Context::set_shared_data(const std::any &data) { shared_data_ = data; }

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

const RequestTargetPtr Context::get_request_target() const {
  return request_target_;
}

http::verb Context::get_request_method() const { return request_method_; }

net::any_io_executor Context::get_executor() { return executor_; }

net::yield_context &Context::get_yield_context() { return yield_; }
};  // namespace blank