#include <boat/log/SyncLogger.h>

#include <boost/log/utility/setup/common_attributes.hpp>

namespace boat::log {

SyncLogger::~SyncLogger() {
  const auto core = boost::log::core::get();

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

void SyncLogger::Init(const int log_level,
                      const std::optional<std::string> log_filename) {
  boost::log::add_common_attributes();
  level_ = parse_severity_level(log_level);
  set_os_log();
  if (log_filename.has_value()) {
    set_file_log(log_filename.value());
  }
  boost::log::core::get()->set_filter(boost::log::trivial::severity >= level_);
}

void SyncLogger::set_os_log() {
  const auto core = boost::log::core::get();

  // create a backend to console
  const auto backend =
      boost::make_shared<boost::log::sinks::text_ostream_backend>();
  backend->add_stream(boost::shared_ptr<std::ostream>(&std::clog));

  // wrap it into the frontend and register in the core
  const boost::shared_ptr<sync_os_sink_t> sink(new sync_os_sink_t(backend));
  os_sink_ = sink;
  core->add_sink(sink);

  // set filtering and formatting
  sink->set_filter(
      boost::log::expressions::attr<boost::log::trivial::severity_level>(
          "Severity") >= level_);
  sink->set_formatter(
      boost::log::expressions::format("[%1%] [%2%] [%3%] %4%") %
      boost::log::expressions::attr<
          boost::log::attributes::local_clock::value_type>("TimeStamp") %
      boost::log::expressions::attr<
          boost::log::attributes::current_thread_id::value_type>("ThreadID") %
      boost::log::expressions::attr<boost::log::trivial::severity_level>(
          "Severity") %
      boost::log::expressions::message);
}

void SyncLogger::set_file_log(const std::string &log_filename) {
  const auto core = boost::log::core::get();

  // create a backend to console
  const auto backend =
      boost::make_shared<boost::log::sinks::text_file_backend>();
  backend->set_file_name_pattern(log_filename + "_%Y%m%d.log");
  backend->set_time_based_rotation(
      boost::log::sinks::file::rotation_at_time_point(0, 0, 0));

  // wrap it into the frontend and register in the core
  const boost::shared_ptr<sync_file_sink_t> sink(new sync_file_sink_t(backend));
  file_sink_ = sink;
  core->add_sink(sink);

  // set filtering and formatting
  sink->set_filter(
      boost::log::expressions::attr<boost::log::trivial::severity_level>(
          "Severity") >= level_);
  sink->set_formatter(
      boost::log::expressions::format("[%1%] [%2%] [%3%] %4%") %
      boost::log::expressions::attr<
          boost::log::attributes::local_clock::value_type>("TimeStamp") %
      boost::log::expressions::attr<
          boost::log::attributes::current_thread_id::value_type>("ThreadID") %
      boost::log::expressions::attr<boost::log::trivial::severity_level>(
          "Severity") %
      boost::log::expressions::message);
}

void SyncLogger::do_logging(const boost::format &formatted,
                            boost::log::trivial::severity_level level) {
  BOOST_LOG_SEV(logger_, level) << formatted;
}

}  // namespace boat::log