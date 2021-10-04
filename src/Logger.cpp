#include "Logger.h"

namespace blank {

Logger::~Logger() { logging::core::get()->flush(); }

void Logger::init(int log_level, const std::string &log_filename) {
    logging::add_common_attributes();
    auto level = parse_log_level(log_level);

    set_os_log();
    if (log_filename.length() > 0) {
        set_file_log(log_filename);
    }

    logging::core::get()->set_filter(logging::trivial::severity >= level);
}

int Logger::trace(const fmt &formatted) {
    return do_logging(formatted, trivial::trace);
}

int Logger::debug(const fmt &formatted) {
    return do_logging(formatted, trivial::debug);
}

int Logger::info(const fmt &formatted) {
    return do_logging(formatted, trivial::info);
}

int Logger::warning(const fmt &formatted) {
    return do_logging(formatted, trivial::warning);
}

int Logger::error(const fmt &formatted) {
    return do_logging(formatted, trivial::error);
}

int Logger::fatal(const fmt &formatted) {
    return do_logging(formatted, trivial::fatal);
}

void Logger::set_os_log() {
    logging::add_console_log(
        std::cout,
        keywords::format =
            "[%BlankHTTP%] [%TimeStamp%] [%ThreadID%] [%Severity%] %Message%");
    logging::core::get()->add_global_attribute(
        "BlankHTTP", attrs::constant<std::string>("BlankHTTP"));
}

void Logger::set_file_log(const std::string &log_filename) {
    logging::add_file_log(
        keywords::file_name = log_filename + "_%Y%m%d.log",
        keywords::time_based_rotation =
            sinks::file::rotation_at_time_point(0, 0, 0),
        keywords::format =
            "[%BlankHTTP%] [%TimeStamp%] [%ThreadID%] [%Severity%] "
            "%Message%");
}

int Logger::do_logging(const fmt &formatted, trivial::severity_level level) {
    BOOST_LOG_SEV(logger_, level) << formatted;
    return formatted.size();
}

trivial::severity_level Logger::parse_log_level(int log_level) {
    if (log_level <= 0)
        return trivial::trace;
    else if (log_level == 1)
        return trivial::debug;
    else if (log_level == 2)
        return trivial::info;
    else if (log_level == 3)
        return trivial::warning;
    else if (log_level == 4)
        return trivial::error;
    else
        return trivial::fatal;
}
};  // namespace blank