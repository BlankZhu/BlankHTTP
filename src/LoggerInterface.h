#pragma once

#define BOOST_LOG_DYN_LINK 1

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/format.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace blank {
namespace logging = boost::log;
namespace trivial = boost::log::trivial;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;
namespace expr = boost::log::expressions;
using fmt = boost::format;

class LoggerInterface {
 public:
  virtual void init(int log_level = 0,
                    const std::string &log_filename = "") = 0;
  void trace(const fmt &formatted) { do_logging(formatted, trivial::trace); }
  void debug(const fmt &formatted) { do_logging(formatted, trivial::debug); }
  void info(const fmt &formatted) { do_logging(formatted, trivial::info); }
  void warning(const fmt &formatted) {
    do_logging(formatted, trivial::warning);
  }
  void error(const fmt &formatted) { do_logging(formatted, trivial::error); }
  void fatal(const fmt &formatted) { do_logging(formatted, trivial::fatal); }

 protected:
  virtual void set_os_log() = 0;
  virtual void set_file_log(const std::string &log_filename) = 0;
  virtual void do_logging(const fmt &formatted,
                          trivial::severity_level level) = 0;
  trivial::severity_level parse_severity_level(int level) {
    if (level <= 0) {
      return trivial::trace;
    }
    if (level == 1) {
      return trivial::debug;
    }
    if (level == 2) {
      return trivial::info;
    }
    if (level == 3) {
      return trivial::warning;
    }
    if (level == 4) {
      return trivial::error;
    }
    return trivial::fatal;
  }
};
using LoggerInterfacePtr = std::shared_ptr<LoggerInterface>;

enum LoggerType { Async, Sync };
}  // namespace blank