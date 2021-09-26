#include "RequestTarget.h"

namespace blank {
void RequestTarget::parse_from_string(const std::string &target) {
    auto path = extract_path_from_target(target);
    path_ = path;

    auto query = extract_query_from_target(target);
    parse_querys(query, query_);

    auto fragment = extract_fragment_from_target(target);
    fragment_ = fragment;
}

std::string RequestTarget::get_path() const { return path_; }

std::string RequestTarget::get_decoded_path() const {
    return percent_decode(get_path());
}

std::vector<std::string> RequestTarget::get_query(
    const std::string &key) const {
    auto found = query_.find(key);
    if (found == query_.end()) {
        return {};
    }
    return found->second;
}

std::vector<std::string> RequestTarget::get_decoded_query(
    const std::string &key) const {
    auto found = get_query(key);
    for (auto &value : found) {
        value = percent_decode(value);
    }
    return found;
}

std::string RequestTarget::get_fragment() const { return fragment_; }

std::string RequestTarget::get_decoded_fragment() const {
    return percent_decode(get_fragment());
}

std::string RequestTarget::extract_path_from_target(
    const std::string &target) const {
    std::vector<std::string> splited{};
    boost::split(splited, target, boost::is_any_of("?"));
    if (!splited.empty()) {
        return splited[0];
    }
    return "";
}

std::string RequestTarget::extract_query_from_target(
    const std::string &target) const {
    // find question mark
    auto question_pos = target.find_first_of('?');
    if (question_pos == std::string::npos) {
        return "";
    }

    // find sharp mark, if possible
    auto sharp_pos = target.find_first_of('#');
    if (sharp_pos < question_pos) {  // invalid
        return "";
    }

    // get the query part
    if (sharp_pos == std::string::npos) {
        return target.substr(question_pos);
    }
    return target.substr(question_pos, sharp_pos - question_pos);
}

std::string RequestTarget::extract_fragment_from_target(
    const std::string &target) const {
    std::vector<std::string> splited{};
    boost::split(splited, target, boost::is_any_of("#"));
    if (splited.size() == 2) {
        return splited[1];
    }
    return "";
}

void RequestTarget::parse_querys(const std::string &query, Query &ret) {
    std::vector<std::string> splited{};
    boost::split(splited, query, boost::is_any_of("&"));

    for (const auto &part : splited) {
        // splited by '='
        std::vector<std::string> key_to_values{};
        boost::split(key_to_values, part, boost::is_any_of("="));
        if (key_to_values.size() != 2) {
            continue;
        }
        auto key = key_to_values[0];
        auto value = key_to_values[1];

        // splited by ','
        std::vector<std::string> values{};
        boost::split(values, value, boost::is_any_of(","));

        // set to ret
        ret.insert_or_assign(key, values);
    }
}
};  // namespace blank