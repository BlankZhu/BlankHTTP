#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/algorithm/string.hpp>

namespace blank
{
    using Query = std::unordered_map<std::string, std::vector<std::string>>;

    // BlankHttp
    class BlankHttpRequestTarget
    {
    public:
        BlankHttpRequestTarget() = default;
        ~BlankHttpRequestTarget() = default;

    public:
        void parse_from_string(const std::string &target);
        std::string get_path() const;
        std::vector<std::string> get_query(const std::string &key) const;
        std::string get_fragment() const;

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

    using BlankHttpRequestTargetPtr = std::shared_ptr<BlankHttpRequestTarget>;
};