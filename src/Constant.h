#pragma once

#include <cstdint>

namespace blank {
namespace constant {
constexpr char const *k_server_name = "BlankHTTP";
constexpr char const *k_log_filename = "access";

constexpr char const *k_mime_html = "text/html";
constexpr char const *k_mime_txt = "text/plain";
constexpr char const *k_mime_json = "application/json";

const std::uint64_t k_default_request_header_limit = 8 * 1024;  // 8 KB
const std::uint64_t k_default_request_body_limit = 64 * 1024;   // 64 KB
const std::uint64_t k_default_response_header_limit = 8 * 1024; // 8 KB
const std::uint64_t k_default_response_body_limit = 64 * 1024;  // 64 KB

const int k_default_log_level = 1;  // debug
constexpr char const * k_default_bind_address = "0.0.0.0";
const unsigned int k_default_port = 8077;
const int k_default_timeout = 30;    // 30 sec
const bool k_default_enable_ssl = false;
};  // namespace constant
};  // namespace blank