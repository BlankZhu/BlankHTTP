#ifndef BOAT_LOG_LOGGERINTERFACE_H
#define BOAT_LOG_LOGGERINTERFACE_H

#include <optional>

#include <boost/format.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>

namespace boat::log {

class LoggerInterface {
 public:
  virtual ~LoggerInterface();

  virtual void Init(int log_level, std::optional<std::string> log_filename) = 0;
  void Trace(const boost::format &formatted) {
    do_logging(formatted, boost::log::trivial::trace);
  }
  void Debug(const boost::format &formatted) {
    do_logging(formatted, boost::log::trivial::debug);
  }
  void Info(const boost::format &formatted) {
    do_logging(formatted, boost::log::trivial::info);
  }
  void Warning(const boost::format &formatted) {
    do_logging(formatted, boost::log::trivial::warning);
  }
  void Error(const boost::format &formatted) {
    do_logging(formatted, boost::log::trivial::error);
  }
  void Fatal(const boost::format &formatted) {
    do_logging(formatted, boost::log::trivial::fatal);
  }

 protected:
  virtual void set_os_log() = 0;
  virtual void set_file_log(const std::string &log_filename) = 0;
  virtual void do_logging(const boost::format &formatted,
                          boost::log::trivial::severity_level level) = 0;
  static boost::log::trivial::severity_level parse_severity_level(
      const int level) {
    if (level <= 0) {
      return boost::log::trivial::trace;
    }
    if (level == 1) {
      return boost::log::trivial::debug;
    }
    if (level == 2) {
      return boost::log::trivial::info;
    }
    if (level == 3) {
      return boost::log::trivial::warning;
    }
    if (level == 4) {
      return boost::log::trivial::error;
    }
    return boost::log::trivial::fatal;
  }
};
using LoggerInterfacePtr = std::shared_ptr<LoggerInterface>;

enum LoggerType { Async, Sync };

}  // namespace boat::log

#endif