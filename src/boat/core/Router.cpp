#include <boat/core/Router.h>

#include <utility>

#include <boost/algorithm/string.hpp>

namespace boat {

void Router::add_handler(const std::string &path,
                         const boost::beast::http::verb &method,
                         HandlerPtr handler) const {
  auto tmp_path = add_leading_slash(path);
  std::vector<std::string> pieces{};
  boost::split(pieces, tmp_path, boost::is_any_of("/"));

  auto curr = root_;
  for (const auto &piece : pieces) {
    if (piece.empty()) {
      continue;
    }

    auto found = curr->node_map.find(piece);
    if (piece.length() >= 2 && piece[0] == ':') {
      found = curr->node_map.find(":");
    }

    // check if need to add a new node
    if (found == curr->node_map.end()) {
      // add a new node
      auto new_node = std::make_shared<RouteNode>();
      // check if current piece is a wildcard
      if (piece.length() >= 2 && piece[0] == ':') {
        new_node->wildcard = piece.substr(1);
        curr->node_map.insert_or_assign(":", new_node);
      } else {
        curr->node_map.insert_or_assign(piece, new_node);
      }
      curr = new_node;
    } else {
      curr = found->second;
    }
  }

  // handle tailling slash here
  if (pieces.back().empty()) {
    auto tail_node = std::make_shared<RouteNode>();
    curr->node_map.insert_or_assign("", tail_node);
    curr = tail_node;
  }

  const int method_int = static_cast<int>(method);
  curr->handler_map[method_int] = std::move(handler);
}

HandlerPtr Router::get_handler(const ContextPtr &context) const {
  const auto tmp_path = add_leading_slash(context->get_url().path());
  std::vector<std::string_view> pieces{};
  boost::split(pieces, tmp_path, boost::is_any_of("/"));

  const auto curr = root_;
  auto ret = dfs_get_handler_helper(context, pieces, 0, curr);
  if (ret == nullptr) {
    ret = std::make_shared<Handler>();
  }
  return ret;
}

HandlerPtr Router::dfs_get_handler_helper(
    const ContextPtr& context, const std::vector<std::string_view> &pieces,
    std::size_t index, const RouteNodePtr& curr_node) const {
  while (pieces[index].empty() &&
         index < pieces.size() -
                     1)  // skip till the last elem (splited by tailing slash)
  {
    index++;
  }
  if (index >= pieces.size()) {
    // return curr_node->handler;
    const auto method_int = static_cast<int>(context->get_request_method());
    const auto ret = curr_node->handler_map.find(method_int);
    if (ret == curr_node->handler_map.end()) {
      return nullptr;
    }
    return ret->second;
  }

  const auto &piece = pieces[index];
  // check if match plain text
  auto found = curr_node->node_map.find(std::string{piece});
  if (found != curr_node->node_map.end()) {
    return dfs_get_handler_helper(context, pieces, index + 1, found->second);
  }

  // if current piece is not an empty string, check if match wildcard
  if (!pieces[index].empty()) {
    found = curr_node->node_map.find(":");
    if (found != curr_node->node_map.end()) {
      auto ret =
          dfs_get_handler_helper(context, pieces, index + 1, found->second);
      if (ret != nullptr) {
        context->set_param(found->second->wildcard, std::string{piece});
      }
      return ret;
    }
  }

  // not found, and index still doesn't reach
  return nullptr;
}

std::string Router::add_leading_slash(const std::string &path) {
  if (!path.empty() && path[0] != '/') {
    return "/" + path;
  }
  return path;
}

}  // namespace boat
