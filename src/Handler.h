#pragma once

#include <any>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include "Constant.h"
#include "Context.h"
#include "Response.h"

namespace blank {
namespace beast = boost::beast;
namespace http = boost::beast::http;

using Request = http::request<http::string_body>;

class Handler {
 public:
  Handler() = default;
  virtual ~Handler() = default;

 public:
  std::any get_extra_data() const;
  void set_extra_data(const std::any &data);

  virtual Response handle_request(ContextPtr ctx, Request &&req);

 private:
  Response base_not_found(ContextPtr ctx, Request &&req);
  std::any extra_data_;
};

using HandlerPtr = std::shared_ptr<Handler>;
};  // namespace blank
