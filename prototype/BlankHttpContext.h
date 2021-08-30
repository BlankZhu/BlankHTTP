#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>

#include "BlankHttpRequestTarget.h"

namespace blank
{
    namespace net = boost::asio;
    using tcp = boost::asio::ip::tcp;
    using Param = std::unordered_map<std::string, std::string>;

    class BlankHttpContext
    {
    public:
        BlankHttpContext(tcp::endpoint endpoint, BlankHttpRequestTargetPtr target, net::any_io_executor executor, net::yield_context &yield)
            : remote_endpoint_(endpoint), request_target_(target), executor_(executor), yield_(yield) {}
        ~BlankHttpContext() = default;

    public:
        void set_param(const std::string &key, const std::string &value);
        std::string get_param(const std::string &key) const;
        std::string get_path() const;
        std::vector<std::string> get_query(const std::string &key) const;
        std::string get_fragment() const;
        net::any_io_executor get_executor();
        net::yield_context &get_yield_context();

    private:
        Param param_;
        tcp::endpoint remote_endpoint_;
        BlankHttpRequestTargetPtr request_target_;
        net::any_io_executor executor_;
        net::yield_context &yield_;
    };

    using BlankHttpContextPtr = std::shared_ptr<BlankHttpContext>;
};