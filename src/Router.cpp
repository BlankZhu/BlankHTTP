#include "Router.h"

namespace blank {
void Router::add_handler(const std::string &path, const http::verb &method,
                         HandlerPtr handler) {
  auto tmp_path = add_leading_slash(path);
  std::vector<std::string> pieces{};
  boost::split(pieces, tmp_path, boost::is_any_of("/"));

  auto curr = root_;
  for (const auto &piece : pieces) {
    if (piece.length() == 0) {
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
  if (pieces.back().length() == 0) {
    auto tail_node = std::make_shared<RouteNode>();
    curr->node_map.insert_or_assign("", tail_node);
    curr = tail_node;
  }

  int method_int = static_cast<int>(method);
  curr->handler_map[method_int] = handler;
}

HandlerPtr Router::get_handler(ContextPtr context) const {
  auto tmp_path =
      add_leading_slash(context->get_request_target()->get_path().to_string());
  boost::string_view tmp_sv{tmp_path};
  std::vector<boost::string_view> pieces{};
  split_sv(pieces, tmp_sv, "/");

  auto curr = root_;
  auto ret = dfs_get_handler_helper(context, pieces, 0, curr);
  if (ret == nullptr) {
    ret = std::make_shared<Handler>();
  }
  return ret;
}

HandlerPtr Router::dfs_get_handler_helper(
    ContextPtr context, const std::vector<boost::string_view> &pieces,
    std::size_t index, RouteNodePtr curr_node) const {
  while (pieces[index].length() == 0 &&
         index < pieces.size() -
                     1)  // skip till the last elem (splited by tailing slash)
  {
    index++;
  }
  if (index >= pieces.size()) {
    // return curr_node->handler;
    auto method_int = static_cast<int>(context->get_request_method());
    auto ret = curr_node->handler_map.find(method_int);
    if (ret == curr_node->handler_map.end()) {
      return nullptr;
    }
    return ret->second;
  }

  const auto &piece = pieces[index];
  // check if match plain text
  auto found = curr_node->node_map.find(piece.to_string());
  if (found != curr_node->node_map.end()) {
    return dfs_get_handler_helper(context, pieces, index + 1, found->second);
  }

  // if current piece is not an empty string, check if match wildcard
  if (pieces[index].length() != 0) {
    found = curr_node->node_map.find(":");
    if (found != curr_node->node_map.end()) {
      auto ret =
          dfs_get_handler_helper(context, pieces, index + 1, found->second);
      if (ret != nullptr) {
        context->set_param(found->second->wildcard, piece.to_string());
      }
      return ret;
    }
  }

  // not found, and index still doesn't reach
  return nullptr;
}

std::string Router::add_leading_slash(const std::string &path) const {
  if (!path.empty() && path[0] != '/') {
    return "/" + path;
  }
  return path;
}

}  // namespace blank
