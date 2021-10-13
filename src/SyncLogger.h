#pragma once

#include "LoggerInterface.h"

namespace blank {
using sync_os_sink_t = sinks::synchronous_sink<sinks::text_ostream_backend>;
using sync_file_sink_t = sinks::synchronous_sink<sinks::text_file_backend>;

class SyncLogger : public LoggerInterface {
 public:
  SyncLogger() = default;
  ~SyncLogger();

  virtual void init(int log_level, const std::string &log_filename);

 protected:
  virtual void set_os_log();
  virtual void set_file_log(const std::string &log_filename);
  virtual void do_logging(const fmt &formatted, trivial::severity_level level);

 private:
  src::severity_logger<trivial::severity_level> logger_;
  trivial::severity_level level_;
  boost::shared_ptr<sync_os_sink_t> os_sink_;
  boost::shared_ptr<sync_file_sink_t> file_sink_;
};
}  // namespace blank