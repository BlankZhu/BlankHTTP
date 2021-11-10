#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/beast/http.hpp>

#include "Context.h"
#include "Handler.h"
#include "Utility.h"

namespace blank {
class RouteTable;
using RouteTableIIPtr = std::shared_ptr<RouteTable>;
class RouteNode;
using RouteNodePtr = std::shared_ptr<RouteNode>;
namespace http = boost::beast::http;

struct Wildcard {
  Wildcard() = default;
  Wildcard(const std::string &key) : key(key) {}

  std::string key;
};

class RouteTable {
 public:
  RouteTable() : root_(std::make_shared<RouteNode>()){};
  ~RouteTable() = default;

 public:
  HandlerPtr get_handler(ContextPtr context) const;
  void add_handler(const std::string &path, const http::verb &method,
                   HandlerPtr handler);

 private:
  HandlerPtr dfs_get_handler_helper(
      ContextPtr context, const std::vector<boost::string_view> &pieces,
      std::size_t index, RouteNodePtr curr_node) const;
  std::string add_leading_slash(const std::string &path) const;

 private:
  RouteNodePtr root_;
};

struct RouteNode {
 public:
  RouteNode() = default;
  ~RouteNode() = default;

 public:
  std::unordered_map<int, HandlerPtr> handler_map;
  bool is_wildcard;
  Wildcard wildcard;
  std::unordered_map<std::string, RouteNodePtr> node_map;
};
};  // namespace blank