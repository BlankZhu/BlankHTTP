#pragma once

#include <unordered_map>

#include "Handler.h"

namespace blank {
struct RouteNode {
 public:
  std::unordered_map<int, HandlerPtr> handler_map;
  std::unordered_map<std::string, std::shared_ptr<RouteNode>> node_map;
  std::string wildcard;
};
using RouteNodePtr = std::shared_ptr<RouteNode>;
}  // namespace blank