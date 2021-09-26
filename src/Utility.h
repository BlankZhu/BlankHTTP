#pragma once

#include <functional>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_set>

#include <boost/io/ios_state.hpp>

namespace blank {
std::string percent_encode(std::string_view to_encode);
std::string percent_decode(std::string_view to_decode);
std::string encode_url(std::string_view url_to_encode);
std::string decode_url(std::string_view url_to_decode);
};  // namespace blank