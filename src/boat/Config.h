#ifndef BOAT_CONFIG_H
#define BOAT_CONFIG_H

#include <cstdint>
#include <memory>
#include <optional>
#include <string>

namespace boat {

class Config {
 public:
  Config() = default;
  ~Config() = default;

  [[nodiscard]] std::string detail_in_json() const;

  // getters & setters
  [[nodiscard]] int get_log_level() const;
  [[nodiscard]] unsigned int get_threads() const;
  [[nodiscard]] std::string get_address() const;
  [[nodiscard]] unsigned short get_port() const;
  [[nodiscard]] unsigned int get_timeout() const;
  [[nodiscard]] std::string get_log_filename() const;
  [[nodiscard]] bool get_enable_ssl() const;
  [[nodiscard]] std::string get_cert_path() const;
  [[nodiscard]] std::string get_pri_key_path() const;
  [[nodiscard]] std::uint32_t get_request_header_limit() const;
  [[nodiscard]] std::uint64_t get_request_body_limit() const;
  [[nodiscard]] bool get_enable_spin() const;

  void set_log_level(int log_level);
  void set_threads(unsigned int threads);
  void set_address(const std::string& address);
  void set_port(unsigned short port);
  void set_timeout(unsigned int timeout);
  void set_log_filename(const std::string& log_filename);
  void set_enable_ssl(bool enable_ssl);
  void set_cert_path(const std::string& cert_path);
  void set_pri_key_path(const std::string& pri_key_path);
  void set_request_header_limit(std::uint32_t limit);
  void set_request_body_limit(std::uint64_t limit);
  void set_enable_spin(bool enable_spin);

 private:
  std::optional<int> log_level_;
  std::optional<unsigned int> threads_;
  std::optional<std::string> address_;
  std::optional<unsigned int> port_;
  std::optional<unsigned int> timeout_;
  std::optional<std::string> log_filename_;
  std::optional<bool> enable_ssl_;
  std::optional<std::string> cert_path_;
  std::optional<std::string> pri_key_path_;
  std::optional<std::uint32_t> request_header_limit_;
  std::optional<std::uint64_t> request_body_limit_;
  std::optional<bool> enable_spin_;
};

using ConfigPtr = std::shared_ptr<Config>;

}  // namespace blank

#endif