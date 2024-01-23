#pragma once

#include <boost/utility/string_view_fwd.hpp>
#include <memory>

#include <boost/beast/http.hpp>

#include "Context.h"
#include "HandleChain.h"
#include "Handler.h"
#include "RouteNode.h"

namespace blank {
namespace http = boost::beast::http;

class Router {
 public:
  Router() : root_(std::make_shared<RouteNode>()){};
  ~Router() = default;

 public:
  void add_handler(const std::string &path, const http::verb &method,
                   HandlerPtr handler);
  HandlerPtr get_handler(ContextPtr context) const;

 private:
  HandlerPtr dfs_get_handler_helper(
      ContextPtr ctx, const std::vector<boost::string_view> &pieces,
      std::size_t index, RouteNodePtr curr_node) const;
  std::string add_leading_slash(const std::string &path) const;

 private:
  RouteNodePtr root_;
};

using RouterPtr = std::shared_ptr<Router>;
}  // namespace blank