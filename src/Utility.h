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

template <typename ContainerT, typename StringViewT, typename DelimiterT>
void split_sv(ContainerT &output, const StringViewT &input,
              const DelimiterT &delimiters) {
  auto first = std::cbegin(input);

  while (true) {
    const auto second =
        std::find_first_of(first, std::cend(input), std::cbegin(delimiters),
                           std::cend(delimiters));
    StringViewT tmp{first, static_cast<std::size_t>(second - first)};
    output.emplace_back(tmp);
    if (second == std::cend(input)) {
      break;
    }
    first = std::next(second);
  }
}
};  // namespace blank