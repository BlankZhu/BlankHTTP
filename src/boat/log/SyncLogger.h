#ifndef BOAT_LOG_SYNCLOGGER_H
#define BOAT_LOG_SYNCLOGGER_H

#include <boat/log/LoggerInterface.h>

namespace boat::log {

using sync_os_sink_t = boost::log::sinks::synchronous_sink<
    boost::log::sinks::text_ostream_backend>;
using sync_file_sink_t =
    boost::log::sinks::synchronous_sink<boost::log::sinks::text_file_backend>;

class SyncLogger final : public LoggerInterface {
 public:
  SyncLogger() = default;
  ~SyncLogger() override;

  void Init(int log_level, std::optional<std::string> log_filename) override;

 protected:
  void set_os_log() override;
  void set_file_log(const std::string &log_filename) override;
  void do_logging(const boost::format &formatted,
                  boost::log::trivial::severity_level level) override;

 private:
  boost::log::sources::severity_logger<boost::log::trivial::severity_level>
      logger_;
  boost::log::trivial::severity_level level_{boost::log::trivial::info};
  boost::shared_ptr<sync_os_sink_t> os_sink_;
  boost::shared_ptr<sync_file_sink_t> file_sink_;
};

}  // namespace boat::log

#endif