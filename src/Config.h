#pragma once

#include <sstream>
#include <string>
#include <thread>

#include <boost/json.hpp>

namespace blank
{
    struct Config
    {
    public:
        void calibrate();
        std::string to_readable_string() const;
        std::string to_json_string() const;
    
    public:
        int log_level;
        int threads;
        std::string address;
        unsigned short port;
        int timeout;
    };
};