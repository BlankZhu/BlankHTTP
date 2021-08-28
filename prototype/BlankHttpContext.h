#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <boost/asio/ip/tcp.hpp>

#include "BlankHttpRequestTarget.h"

namespace blank
{
    using tcp = boost::asio::ip::tcp;
    using Param = std::unordered_map<std::string, std::string>;

    class BlankHttpContext
    {
    public:
        BlankHttpContext(tcp::endpoint endpoint, BlankHttpRequestTargetPtr target)
            : remote_endpoint_(endpoint), request_target_(target) {}
        ~BlankHttpContext() = default;

    public:
        void set_param(const std::string &key, const std::string &value);
        std::string get_param(const std::string &key) const;
        std::string get_path() const;
        std::vector<std::string> get_query(const std::string &key) const;
        std::string get_fragment() const;

    private:
        Param param_;
        tcp::endpoint remote_endpoint_;
        BlankHttpRequestTargetPtr request_target_;
    };

    using BlankHttpContextPtr = std::shared_ptr<BlankHttpContext>;
};