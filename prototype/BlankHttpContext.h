#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <boost/asio/spawn.hpp>
#include <boost/asio/ip/tcp.hpp>

#include "BlankHttpRequestTarget.h"

namespace blank
{
    namespace net = boost::asio;
    using tcp = boost::asio::ip::tcp;
    using Param = std::unordered_map<std::string, std::string>;

    class BlankHttpContext
    {
    public:
        BlankHttpContext(tcp::endpoint endpoint, BlankHttpRequestTargetPtr target, net::yield_context &yield)
            : remote_endpoint_(endpoint), request_target_(target), yield_(yield) {}
        ~BlankHttpContext() = default;

    public:
        void set_param(const std::string &key, const std::string &value);
        std::string get_param(const std::string &key) const;
        std::string get_path() const;
        std::vector<std::string> get_query(const std::string &key) const;
        std::string get_fragment() const;
        net::yield_context &get_yield_context();

    private:
        Param param_;
        tcp::endpoint remote_endpoint_;
        BlankHttpRequestTargetPtr request_target_;
        net::yield_context &yield_;
    };

    using BlankHttpContextPtr = std::shared_ptr<BlankHttpContext>;
};