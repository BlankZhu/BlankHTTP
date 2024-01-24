#ifndef BOAT_CONSTANT_H
#define BOAT_CONSTANT_H

#include <cstdint>

namespace boat::constant {

constexpr char const *k_server_name = "BlankHTTP";
constexpr char const *k_log_filename = "access";

constexpr char const *k_mime_html = "text/html";
constexpr char const *k_mime_txt = "text/plain";
constexpr char const *k_mime_json = "application/json";

constexpr std::uint32_t k_default_request_header_limit = 8 * 1024;  // 8 KB
constexpr std::uint64_t k_default_request_body_limit = 64 * 1024;   // 64 KB

constexpr int k_default_log_level = 1;  // debug
constexpr char const *k_default_bind_address = "0.0.0.0";
constexpr unsigned int k_default_port = 8077;
constexpr int k_default_timeout = 30;  // 30 sec
constexpr bool k_default_enable_ssl = false;
constexpr bool k_default_enable_spin = false;

}  // namespace boat::constant

#endif