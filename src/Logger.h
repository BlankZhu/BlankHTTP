#pragma once

#define BOOST_LOG_DYN_LINK 1

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include "Constant.h"

namespace blank
{
    namespace logging = boost::log;
    namespace trivial = boost::log::trivial;
    namespace keywords = boost::log::keywords;
    namespace attrs = boost::log::attributes;

    class Logger
    {
    public:
        // static void init(trivial::severity_level log_level = trivial::trace);
        static void init(int log_level = 0);
    private:
        static trivial::severity_level parse_log_level(int log_level);
    };
};