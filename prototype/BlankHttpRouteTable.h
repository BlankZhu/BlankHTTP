#pragma once

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "BlankHttpContext.h"
#include "BlankHttpHandler.h"

namespace blank
{
    class BlankHttpRouteTable;
    using BlankHttpRouteTablePtr = std::shared_ptr<BlankHttpRouteTable>;
    class BlankHttpRouteNode;
    using BlankHttpRouteNodePtr = std::shared_ptr<BlankHttpRouteNode>;

    struct Wildcard
    {
        Wildcard() = default;
        Wildcard(const std::string &key) : key(key) {}

        std::string key;
    };

    class BlankHttpRouteTable
    {
    public:
        BlankHttpRouteTable() : root_(std::make_shared<BlankHttpRouteNode>()){};
        ~BlankHttpRouteTable() = default;

    public:
        BlankHttpHandlerPtr get_handler(BlankHttpContextPtr context) const;
        void add_handler(const std::string &path, BlankHttpHandlerPtr handler);

    private:
        BlankHttpHandlerPtr dfs_get_handler_helper(
            BlankHttpContextPtr context,
            const std::vector<std::string> &pieces,
            std::size_t index,
            BlankHttpRouteNodePtr curr_node) const;
        std::string add_leading_slash(const std::string &path) const;

    private:
        BlankHttpRouteNodePtr root_;
    };

    struct BlankHttpRouteNode
    {
    public:
        BlankHttpRouteNode() : handler(nullptr){};
        ~BlankHttpRouteNode() = default;

    public:
        BlankHttpHandlerPtr handler;
        bool is_wildcard;
        Wildcard wildcard;
        std::unordered_map<std::string, BlankHttpRouteNodePtr> node_map;
    };
};