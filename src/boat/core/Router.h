#ifndef BOAT_CORE_ROUTER_H
#define BOAT_CORE_ROUTER_H

#include <memory>

#include <boost/beast/http.hpp>

#include <boat/core/Context.h>
#include <boat/core/Handler.h>
#include <boat/core/RouteNode.h>

namespace boat {

class Router {
 public:
  Router() : root_(std::make_shared<RouteNode>()){};
  ~Router() = default;

  void add_handler(const std::string &path,
                   const boost::beast::http::verb &method,
                   HandlerPtr handler) const;
  [[nodiscard]] HandlerPtr get_handler(const ContextPtr& context) const;

 private:
  [[nodiscard]] HandlerPtr dfs_get_handler_helper(
      const ContextPtr& context, const std::vector<std::string_view> &pieces,
      std::size_t index, const RouteNodePtr& curr_node) const;
  [[nodiscard]] static std::string add_leading_slash(const std::string &path);

  RouteNodePtr root_;
};

using RouterPtr = std::shared_ptr<Router>;

}  // namespace boat

#endif