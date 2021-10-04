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
    void init(int log_level = 1, const std::string &log_filename = "");
    int trace(const fmt &formatted);
    int debug(const fmt &formatted);
    int info(const fmt &formatted);
    int warning(const fmt &formatted);
    int error(const fmt &formatted);
    int fatal(const fmt &formatted);

   private:
    void set_os_log();
    void set_file_log(const std::string &log_filename);
    int do_logging(const fmt &formatted, trivial::severity_level level);
    trivial::severity_level parse_log_level(int log_level);

   private:
    sources::severity_logger<trivial::severity_level> logger_;
};
};  // namespace blank