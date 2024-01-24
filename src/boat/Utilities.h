#ifndef BOAT_UTILITIES_H
#define BOAT_UTILITIES_H

#include <string>

#include <boost/url.hpp>

namespace boat::utilities {

std::string percent_encode(std::string_view to_encode);
std::string percent_decode(std::string_view to_decode);
std::string encode_url(std::string_view url_to_encode);
std::string decode_url(std::string_view url_to_decode);

}  // namespace boat

#endif