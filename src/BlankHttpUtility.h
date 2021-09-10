#pragma once

#include <functional>
#include <iomanip>
#include <sstream>
#include <string>
#include <unordered_set>

#include <boost/io/ios_state.hpp>

namespace blank
{
    std::string encode_url(std::string_view to_encode);
    std::string decode_url(std::string_view to_decode);
};