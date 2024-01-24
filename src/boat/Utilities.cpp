#include <boat/Utilities.h>

namespace boat::utilities {

std::string percent_encode(const std::string_view to_encode) {
  return boost::urls::encode(to_encode, boost::urls::unreserved_chars);
}

std::string percent_decode(const std::string_view to_decode) {
  if (boost::system::result<boost::urls::pct_string_view> rs =
          boost::urls::make_pct_string_view(to_decode);
      rs.has_value()) {
    return rs.value().data();
  }
  return "";
}

std::string encode_url(const std::string_view url_to_encode) {
  return percent_encode(url_to_encode);
}

std::string decode_url(const std::string_view url_to_decode) {
  return percent_decode(url_to_decode);
}

}  // namespace boat::utilities