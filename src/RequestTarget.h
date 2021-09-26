#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "Utility.h"

namespace blank {
using Query = std::unordered_map<std::string, std::vector<std::string>>;

class RequestTarget {
   public:
    RequestTarget() = default;
    ~RequestTarget() = default;

   public:
    void parse_from_string(const std::string &target);
    std::string get_path() const;
    std::string get_decoded_path() const;
    std::vector<std::string> get_query(const std::string &key) const;
    std::vector<std::string> get_decoded_query(const std::string &key) const;
    std::string get_fragment() const;
    std::string get_decoded_fragment() const;

   private:
    std::string extract_path_from_target(const std::string &target) const;
    std::string extract_query_from_target(const std::string &target) const;
    std::string extract_fragment_from_target(const std::string &target) const;
    void parse_querys(const std::string &query, Query &ret);

   private:
    std::string path_;
    Query query_;
    std::string fragment_;
};

using RequestTargetPtr = std::shared_ptr<RequestTarget>;
};  // namespace blank