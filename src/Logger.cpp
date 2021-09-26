#include "Logger.h"

namespace blank {

Logger::~Logger() { logging::core::get()->flush(); }

void Logger::init(int log_level) {
    // setup console log here
    auto level = parse_log_level(log_level);
    logging::add_console_log(
        std::cout,
        keywords::format =
            "[%BlankHTTP%] [%TimeStamp%] [%ThreadID%] [%Severity%] %Message%");
    logging::core::get()->add_global_attribute(
        "BlankHTTP", attrs::constant<std::string>("BlankHTTP"));

    logging::add_common_attributes();
}

void Logger::init(int log_level, const std::string &log_filename) {
    if (log_filename.length() != 0) {
        // setup file log here
        logging::add_file_log(
            keywords::file_name = log_filename + "_%Y%m%d.log",
            keywords::time_based_rotation =
                sinks::file::rotation_at_time_point(0, 0, 0),
            keywords::format =
                "[%BlankHTTP%] [%TimeStamp%] [%ThreadID%] [%Severity%] "
                "%Message%");
    }

    init(log_level);
}

int Logger::trace(fmt formatted) {
    return do_logging(formatted, trivial::trace);
}

int Logger::debug(fmt formatted) {
    return do_logging(formatted, trivial::debug);
}

int Logger::info(fmt formatted) { return do_logging(formatted, trivial::info); }

int Logger::warning(fmt formatted) {
    return do_logging(formatted, trivial::warning);
}

int Logger::error(fmt formatted) {
    return do_logging(formatted, trivial::error);
}

int Logger::fatal(fmt formatted) {
    return do_logging(formatted, trivial::fatal);
}

int Logger::do_logging(fmt &formatted, trivial::severity_level level) {
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