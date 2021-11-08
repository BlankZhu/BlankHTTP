#include "RequestTarget.h"
#include <boost/utility/string_view_fwd.hpp>
#include "Utility.h"

namespace blank {
void RequestTarget::parse_from_string(boost::string_view target) {
  auto path = extract_path_from_target(target);
  path_ = path;

  query_raw_ = extract_query_from_target(target);
  parse_querys(query_raw_, query_);

  auto fragment = extract_fragment_from_target(target);
  fragment_ = fragment;
}

boost::string_view RequestTarget::get_path() const { return path_; }

std::string RequestTarget::get_decoded_path() const {
  return percent_decode({path_.data(), path_.size()});
}

boost::string_view RequestTarget::get_query() const { return query_raw_; }

std::vector<boost::string_view> RequestTarget::get_query(
    const std::string &key) const {
  auto found = query_.find(key);
  if (found == query_.end()) {
    return {};
  }
  return found->second;
}

std::string RequestTarget::get_decoded_query() const {
  return percent_decode({query_raw_.data(), query_raw_.size()});
}

std::vector<std::string> RequestTarget::get_decoded_query(
    const std::string &key) const {
  std::vector<std::string> ret{};
  auto found = get_query(key);
  for (const auto &value : found) {
    ret.emplace_back(percent_decode({value.data(), value.size()}));
  }
  return ret;
}

boost::string_view RequestTarget::get_fragment() const { return fragment_; }

std::string RequestTarget::get_decoded_fragment() const {
  return percent_decode({fragment_.data(), fragment_.size()});
}

boost::string_view RequestTarget::extract_path_from_target(
    boost::string_view target) const {
  std::vector<boost::string_view> splitted{};
  // boost::split(splitted, target, boost::is_any_of("?"));
  split_sv(splitted, target, "?");
  if (!splitted.empty()) {
    return splitted[0];
  }
  return "";
}

boost::string_view RequestTarget::extract_query_from_target(
    boost::string_view target) const {
  // find question mark
  auto question_pos = target.find_first_of('?');
  if (question_pos == boost::string_view::npos) {
    return "";
  }

  // find sharp mark, if possible
  auto sharp_pos = target.find_first_of('#');
  if (sharp_pos < question_pos) {  // invalid
    return "";
  }

  // get the query part
  if (sharp_pos == boost::string_view::npos) {
    return target.substr(question_pos + 1);
  }
  return target.substr(question_pos + 1, sharp_pos - question_pos - 1);
}

boost::string_view RequestTarget::extract_fragment_from_target(
    boost::string_view target) const {
  std::vector<boost::string_view> splitted{};
  // boost::split(splitted, target, boost::is_any_of("#"));
  split_sv(splitted, target, "#");
  if (splitted.size() == 2) {
    return splitted[1];
  }
  return "";
}

void RequestTarget::parse_querys(boost::string_view query, Query &ret) {
  std::vector<boost::string_view> splitted{};
  // boost::split(splitted, query, boost::is_any_of("&"));
  split_sv(splitted, query, "&");

  for (const auto &part : splitted) {
    // splitted by '='
    std::vector<std::string> key_to_values{};
    boost::split(key_to_values, part, boost::is_any_of("="));
    if (key_to_values.size() != 2) {
      continue;
    }
    auto key = key_to_values[0];
    auto value = boost::string_view{key_to_values[1]};

    // splitted by ','
    std::vector<boost::string_view> values{};
    split_sv(values, value, ",");

    // set to ret
    ret.insert_or_assign(key, values);
  }
}
};  // namespace blank