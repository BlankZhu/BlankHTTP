#include "BlankHttpLogger.h"

namespace blank
{
    void BlankHttpLogger::init(int log_level)
    {
        auto lv = parse_log_level(log_level);
    
        // logging::register_simple_formatter_factory<trivial::severity_level, char>("Severity");
        logging::add_console_log(
            std::cout,
            keywords::format = "[%Blank%] [%TimeStamp%] [%ThreadID%] [%Severity%] %Message%");

        logging::core::get()->set_filter(trivial::severity >= lv);
        logging::core::get()->add_global_attribute("Blank", attrs::constant<std::string>(constant::k_server_name));
        logging::add_common_attributes();
    }

    trivial::severity_level BlankHttpLogger::parse_log_level(int log_level)
    {
        if (log_level <= 0)
        {
            return trivial::severity_level::trace;
        }
        if (log_level == 1)
        {
            return trivial::severity_level::debug;
        }
        if (log_level == 2)
        {
            return trivial::severity_level::info;
        }
        if (log_level == 3)
        {
            return trivial::severity_level::warning;
        }
        if (log_level == 4)
        {
            return trivial::severity_level::error;
        }
        return trivial::severity_level::fatal;
    }
};