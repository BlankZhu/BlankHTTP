#pragma once

#define BOOST_LOG_DYN_LINK 1

#include "LoggerInterface.h"

namespace blank {
class CoreLogger : public LoggerInterface {
 public:
  CoreLogger() = default;
  ~CoreLogger();

  virtual void init(int log_level = 0, const std::string &log_filename = "");

 protected:
  virtual void set_os_log();
  virtual void set_file_log(const std::string &log_filename);
  virtual void do_logging(const fmt &formatted, trivial::severity_level level);

 private:
  src::severity_logger<trivial::severity_level> logger_;
};
}  // namespace blank
