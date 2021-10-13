#include "RouteTable.h"

namespace blank {
void RouteTable::add_handler(const std::string &path, const http::verb &method,
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
        new_node->is_wildcard = true;
        new_node->wildcard = Wildcard{piece.substr(1)};
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
    tail_node->is_wildcard = false;
    curr->node_map.insert_or_assign("", tail_node);
    curr = tail_node;
  }

  int method_int = static_cast<int>(method);
  curr->handler_map[method_int] = handler;
}

HandlerPtr RouteTable::get_handler(ContextPtr context) const {
  auto tmp_path = add_leading_slash(context->get_path());
  std::vector<std::string> pieces{};
  boost::split(pieces, tmp_path, boost::is_any_of("/"));

  auto curr = root_;
  for (const auto &piece : pieces) {
    // skip empty pieces
    if (piece.size() == 0) {
      continue;
    }

    // check if match plain text
    auto match_plain_text = curr->node_map.find(piece);
    if (match_plain_text != curr->node_map.end()) {
      // match, continue to next node
      curr = match_plain_text->second;
      continue;
    }

    // if current piece is not an empty string, check if match wildcard
    if (piece.length() != 0) {
      auto match_wildcard = curr->node_map.find(":");
      if (match_wildcard != curr->node_map.end()) {
        // match, continue to next node
        context->set_param(curr->wildcard.key, piece);
        curr = match_wildcard->second;
        continue;
      }
    }

    // no match at all, stop matching and return
    return nullptr;
  }

  // not nullptr, check if any method match
  auto method = static_cast<int>(context->get_method());
  auto match_method = curr->handler_map.find(method);
  if (match_method == curr->handler_map.end()) {
    return nullptr;
  }

  // return the match result
  return match_method->second;
}

std::string RouteTable::add_leading_slash(const std::string &path) const {
  if (!path.empty() && path[0] != '/') {
    return "/" + path;
  }
  return path;
}
};  // namespace blank