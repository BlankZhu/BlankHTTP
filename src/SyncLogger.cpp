#include "SyncLogger.h"

SyncLogger::~SyncLogger() {
  auto core = logging::core::get();

  if (os_sink_ != nullptr) {
    core->remove_sink(os_sink_);
    os_sink_->flush();
    os_sink_.reset();
  }
  if (file_sink_ != nullptr) {
    core->remove_sink(file_sink_);
    file_sink_->flush();
    file_sink_.reset();
  }

  core->flush();
}

void SyncLogger::init(int log_level, const std::string &log_filename) {
  logging::add_common_attributes();
  level_ = parse_severity_level(log_level);
  set_os_log();
  if (log_filename.length() > 0) {
    set_file_log(log_filename);
  }
  logging::core::get()->set_filter(logging::trivial::severity >= level_);
}

void SyncLogger::set_os_log() {
  auto core = logging::core::get();

  // create a backend to console
  auto backend = boost::make_shared<sinks::text_ostream_backend>();
  backend->add_stream(
      boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));

  // wrap it into the frontend and register in the core
  boost::shared_ptr<sync_os_sink_t> sink(new sync_os_sink_t(backend));
  os_sink_ = sink;
  core->add_sink(sink);

  // set filtering and formatting
  sink->set_filter(expr::attr<trivial::severity_level>("Severity") >= level_);
  sink->set_formatter(
      expr::format("[%1%] [%2%] [%3%] %4%") %
      expr::attr<attrs::local_clock::value_type>("TimeStamp") %
      expr::attr<attrs::current_thread_id::value_type>("ThreadID") %
      expr::attr<trivial::severity_level>("Severity") % expr::message);
}

void SyncLogger::set_file_log(const std::string &log_filename) {
  auto core = logging::core::get();

  // create a backend to console
  auto backend = boost::make_shared<sinks::text_file_backend>();
  backend->set_file_name_pattern(log_filename + "_%Y%m%d.log");
  backend->set_time_based_rotation(
      sinks::file::rotation_at_time_point(0, 0, 0));

  // wrap it into the frontend and register in the core
  boost::shared_ptr<sync_file_sink_t> sink(new sync_file_sink_t(backend));
  file_sink_ = sink;
  core->add_sink(sink);

  // set filtering and formatting
  sink->set_filter(expr::attr<trivial::severity_level>("Severity") >= level_);
  sink->set_formatter(
      expr::format("[%1%] [%2%] [%3%] %4%") %
      expr::attr<attrs::local_clock::value_type>("TimeStamp") %
      expr::attr<attrs::current_thread_id::value_type>("ThreadID") %
      expr::attr<trivial::severity_level>("Severity") % expr::message);
}

void SyncLogger::do_logging(const fmt &formatted,
                            trivial::severity_level level) {
  BOOST_LOG_SEV(logger_, level);
}