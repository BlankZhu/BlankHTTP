#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/utility/string_view.hpp>

#include "Utility.h"

namespace blank {
using Query = std::unordered_map<std::string, std::vector<boost::string_view>>;

class RequestTarget {
 public:
  RequestTarget(boost::string_view target) { parse_from_string(target); }
  ~RequestTarget() = default;

 public:
  boost::string_view get_path() const;
  std::string get_decoded_path() const;
  boost::string_view get_query() const;
  std::string get_decoded_query() const;
  std::vector<boost::string_view> get_query(const std::string &key) const;
  std::vector<std::string> get_decoded_query(const std::string &key) const;
  boost::string_view get_fragment() const;
  std::string get_decoded_fragment() const;

 private:
  void parse_from_string(boost::string_view target);
  boost::string_view extract_path_from_target(boost::string_view target) const;
  boost::string_view extract_query_from_target(boost::string_view target) const;
  boost::string_view extract_fragment_from_target(
      boost::string_view target) const;
  void parse_querys(boost::string_view query_raw, Query &ret);

 private:
  boost::string_view path_;
  Query query_;
  boost::string_view query_raw_;  // stores the query part before splitting
  boost::string_view fragment_;
};

using RequestTargetPtr = std::shared_ptr<RequestTarget>;
};  // namespace blank