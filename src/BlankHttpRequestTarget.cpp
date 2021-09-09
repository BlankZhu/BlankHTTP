#include "BlankHttpRequestTarget.h"

namespace blank
{
    void BlankHttpRequestTarget::parse_from_string(const std::string &target)
    {
        auto path = extract_path_from_target(target);
        path_ = path;

        auto query = extract_query_from_target(target);
        parse_querys(query, query_);

        auto fragment = extract_fragment_from_target(target);
        fragment_ = fragment;
    }

    std::string BlankHttpRequestTarget::get_path() const
    {
        return path_;
    }

    std::vector<std::string> BlankHttpRequestTarget::get_query(const std::string &key) const
    {
        auto found = query_.find(key);
        if (found == query_.end())
        {
            return {};
        }
        return found->second;
    }

    std::string BlankHttpRequestTarget::get_fragment() const
    {
        return fragment_;
    }

    std::string BlankHttpRequestTarget::extract_path_from_target(const std::string &target) const
    {
        std::vector<std::string> splited{};
        boost::split(splited, target, boost::is_any_of("?"));
        if (!splited.empty())
        {
            return splited[0];
        }
        return "";
    }

    std::string BlankHttpRequestTarget::extract_query_from_target(const std::string &target) const
    {
        // find question mark
        auto question_pos = target.find_first_of('?');
        if (question_pos == std::string::npos)
        {
            return "";
        }

        // find sharp mark, if possible
        auto sharp_pos = target.find_first_of('#');
        if (sharp_pos < question_pos)
        { // invalid
            return "";
        }

        // get the query part
        if (sharp_pos == std::string::npos)
        {
            return target.substr(question_pos);
        }
        return target.substr(question_pos, sharp_pos - question_pos);
    }

    std::string BlankHttpRequestTarget::extract_fragment_from_target(const std::string &target) const
    {
        std::vector<std::string> splited{};
        boost::split(splited, target, boost::is_any_of("#"));
        if (splited.size() == 2)
        {
            return splited[1];
        }
        return "";
    }

    void BlankHttpRequestTarget::parse_querys(const std::string &query, Query &ret)
    {
        std::vector<std::string> splited{};
        boost::split(splited, query, boost::is_any_of("&"));

        for (const auto &part : splited)
        {
            // splited by '='
            std::vector<std::string> key_to_values{};
            boost::split(key_to_values, part, boost::is_any_of("="));
            if (key_to_values.size() != 2)
            {
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
};