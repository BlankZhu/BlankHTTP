#include "CoreLogger.h"

CoreLogger::~CoreLogger() {
  auto core = logging::core::get();
  core->flush();
}

void CoreLogger::init(int log_level, const std::string &log_filename) {
  logging::add_common_attributes();
  auto level = parse_severity_level(log_level);
  set_os_log();
  if (log_filename.length() > 0) {
    set_file_log(log_filename);
  }
  logging::core::get()->set_filter(logging::trivial::severity >= level);
}

void CoreLogger::set_os_log() {
  logging::add_console_log(
      std::cout,
      keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%");
}

void CoreLogger::set_file_log(const std::string &log_filename) {
  logging::add_file_log(
      keywords::file_name = log_filename + "_%Y%m%d.log",
      keywords::time_based_rotation =
          sinks::file::rotation_at_time_point(0, 0, 0),
      keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%");
}

void CoreLogger::do_logging(const fmt &formatted,
                            trivial::severity_level level) {
  BOOST_LOG_SEV(logger_, level);
}