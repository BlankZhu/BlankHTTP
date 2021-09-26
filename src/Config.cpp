#include "Config.h"

namespace blank {
void Config::calibrate() {
    if (address.empty()) {
        address = "0.0.0.0";
    }
    if (port == 0) {
        port = 8077;
    }
    if (timeout <= 0) {
        timeout = 5;
    }
    if (threads == 0) {
        threads = static_cast<int>(std::thread::hardware_concurrency());
    }
    if (log_level > 5) {
        log_level = 5;
    }
    if (log_level < 0) {
        log_level = 0;
    }
}

std::string Config::to_readable_string() const {
    std::stringstream ss;
    ss << "serving at: [" << address << ":" << port << "], "
       << "with HTTP timeout: [" << timeout << "] in second, "
       << "running in thread number: [" << threads << "], "
       << "on log level: [" << log_level << "], "
       << "logging to file: [" << log_filename << "], "
       << "enable ssl: [" << enable_ssl << "]";

    if (enable_ssl) {
        ss << ", "
           << "using server certification: " << cert_path << "], "
           << "using private key: " << pri_key_path << "]";
    }

    return ss.str();
}

std::string Config::to_json_string() const {
    namespace json = boost::json;
    json::object obj{};
    obj["timeout"] = timeout;
    obj["address"] = address;
    obj["port"] = port;
    obj["threads"] = threads;
    obj["log_level"] = log_level;
    obj["log_filename"] = log_filename;
    obj["enable_ssl"] = enable_ssl;

    if (enable_ssl) {
        obj["cert_path"] = cert_path;
        obj["pri_key_path"] = pri_key_path;
    }

    return json::serialize(obj);
}
};  // namespace blank