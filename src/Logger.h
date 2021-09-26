#pragma once

#define BOOST_LOG_DYN_LINK 1

#include <boost/format.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

#include "Constant.h"

namespace blank {
namespace logging = boost::log;
namespace trivial = boost::log::trivial;
namespace sources = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;
using fmt = boost::format;

class Logger {
   public:
    Logger() = default;
    ~Logger();

   public:
    void init(int log_level = 1);
    void init(int log_level, const std::string &log_filename);
    int trace(fmt formatted);
    int debug(fmt formatted);
    int info(fmt formatted);
    int warning(fmt formatted);
    int error(fmt formatted);
    int fatal(fmt formatted);

   private:
    int do_logging(fmt &formatted, trivial::severity_level level);
    trivial::severity_level parse_log_level(int log_level);

   private:
    sources::severity_logger<trivial::severity_level> logger_;
};
};  // namespace blank