#include "BlankHttpConfig.h"

namespace blank
{
    void BlankHttpConfig::calibrate()
    {
        if (address.empty())
        {
            address = "0.0.0.0";
        }
        if (port == 0)
        {
            port = 8077;
        }
        if (timeout <= 0)
        {
            timeout = 5;
        }
        if (threads == 0)
        {
            threads = static_cast<int>(std::thread::hardware_concurrency());
        }
        if (log_level > 5)
        {
            log_level = 5;
        }
        if (log_level < 0)
        {
            log_level = 0;
        }
    }

    std::string BlankHttpConfig::to_readable_string() const
    {
        std::stringstream ss;
        ss << "serving at: [" << address << ":" << port << "], "
           << "with HTTP timeout: [" << timeout << "] in second, "
           << "running in thread number: [" << threads << "], "
           << "on log level: [" << log_level << "]";
        return ss.str();
    }

    std::string BlankHttpConfig::to_json_string() const
    {
        namespace json = boost::json;
        json::object obj{};
        obj["timeout"] = timeout;
        obj["address"] = address;
        obj["port"] = port;
        obj["threads"] = threads;
        obj["log_level"] = log_level;

        return json::serialize(obj);
    }
};