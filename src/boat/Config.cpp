#include <boat/Config.h>

#include <thread>

#include <boost/json.hpp>

#include <boat/Constant.h>

namespace boat {

int Config::get_log_level() const {
  if (log_level_.has_value()) {
    if (log_level_ > 5) {
      return 5;
    }
    if (log_level_ < 0) {
      return 0;
    }
  }
  return log_level_.value_or(constant::k_default_log_level);
}

unsigned int Config::get_threads() const {
  if (threads_.has_value() && threads_ == 0) {
    return std::thread::hardware_concurrency();
  }
  return threads_.value_or(std::thread::hardware_concurrency());
}

std::string Config::get_address() const {
  return address_.value_or(constant::k_default_bind_address);
}

unsigned short Config::get_port() const {
  return port_.value_or(constant::k_default_port);
}

unsigned int Config::get_timeout() const {
  return timeout_.value_or(constant::k_default_timeout);
}

std::string Config::get_log_filename() const {
  return log_filename_.value_or("");
}

bool Config::get_enable_ssl() const {
  return enable_ssl_.value_or(constant::k_default_enable_ssl);
}

std::string Config::get_cert_path() const { return cert_path_.value_or(""); }

std::string Config::get_pri_key_path() const {
  return pri_key_path_.value_or("");
}

std::uint32_t Config::get_request_header_limit() const {
  return request_header_limit_.value_or(
      constant::k_default_request_header_limit);
}

std::uint64_t Config::get_request_body_limit() const {
  return request_body_limit_.value_or(constant::k_default_request_body_limit);
}

bool Config::get_enable_spin() const {
  return enable_spin_.value_or(constant::k_default_enable_spin);
}

void Config::set_log_level(int log_level) { log_level_ = log_level; }

void Config::set_threads(unsigned int threads) { threads_ = threads; }

void Config::set_address(const std::string& address) { address_ = address; }

void Config::set_port(unsigned short port) { port_ = port; }

void Config::set_timeout(unsigned int timeout) { timeout_ = timeout; }

void Config::set_log_filename(const std::string& log_filename) {
  log_filename_ = log_filename;
}

void Config::set_enable_ssl(bool enable_ssl) { enable_ssl_ = enable_ssl; }
void Config::set_cert_path(const std::string& cert_path) {
  cert_path_ = cert_path;
}

void Config::set_pri_key_path(const std::string& pri_key_path) {
  pri_key_path_ = pri_key_path;
}

void Config::set_request_header_limit(std::uint32_t limit) {
  request_header_limit_ = limit;
}

void Config::set_request_body_limit(std::uint64_t limit) {
  request_body_limit_ = limit;
}

void Config::set_enable_spin(bool enable_spin) { enable_spin_ = enable_spin; }

std::string Config::detail_in_json() const {
  namespace json = boost::json;
  json::object obj{};

  obj["log_level"] = get_log_level();
  obj["threads"] = get_threads();
  obj["address"] = get_address();
  obj["port"] = get_port();
  obj["timeout"] = get_timeout();
  obj["log_filename"] = get_log_filename();
  obj["enable_ssl"] = get_enable_ssl();
  obj["cert_path"] = get_cert_path();
  obj["pri_key_path"] = get_pri_key_path();
  obj["request_header_limit"] = get_request_header_limit();
  obj["request_body_limit"] = get_request_body_limit();
  obj["enable_spin"] = get_enable_spin();

  return json::serialize(obj);
}

}  // namespace blank