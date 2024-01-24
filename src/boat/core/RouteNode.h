#ifndef BOAT_CORE_ROUTENODE_H
#define BOAT_CORE_ROUTENODE_H

#include <unordered_map>

#include <boat/core/Handler.h>

namespace boat {

struct RouteNode {
  std::unordered_map<int, HandlerPtr> handler_map;
  std::unordered_map<std::string, std::shared_ptr<RouteNode>> node_map;
  std::string wildcard;
};
using RouteNodePtr = std::shared_ptr<RouteNode>;

}  // namespace boat

#endif