#include "Utility.h"
#include <cstdint>

namespace blank {
std::string percent_encode(std::string_view to_encode) {
    std::stringstream res;

    for (auto iter = to_encode.begin(); iter != to_encode.end(); ++iter) {
        auto c = *iter;

        if (c == ' ') {
            res << "%20";
        } else if (c == '+') {
            res << "%2B";
        } else if (c == '\r') {
            res << "%0D";
        } else if (c == '\n') {
            res << "%0A";
        } else if (c == '\'') {
            res << "%27";
        } else if (c == ',') {
            res << "%2C";
        } else if (c == ';') {
            res << "%3B";
        } else {
            std::uint8_t u8 = static_cast<std::uint8_t>(c);
            if (u8 >= 0x80) {
                boost::io::ios_flags_saver ifs{res};
                res << '%' << std::hex << std::setfill('0') << std::setw(2)
                    << std::uppercase << u8;
            } else {
                res << c;
            }
        }
    }

    return res.str();
}

std::string percent_decode(std::string_view to_decode) {
    std::stringstream ss{};
    static const std::unordered_set<char> hex{
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a',
        'b', 'c', 'd', 'e', 'f', 'A', 'B', 'C', 'D', 'E', 'F'};
    static std::function<bool(char)> is_hex = [](char c) {
        if (hex.find(c) != hex.end()) {
            return true;
        }
        return false;
    };

    for (auto iter = to_decode.begin(); iter != to_decode.end(); ++iter) {
        char c = *iter;
        auto next = iter + 1;
        if (c == '%' && next != to_decode.end()) {
            // check if the next 2 chars combine a hex digit
            auto nextnext = next + 1;
            if (nextnext == to_decode.end()) {
                ss << c;
                continue;
            }
            if (!is_hex(*next) || !is_hex(*nextnext)) {
                ss << c;
                continue;
            }
            // combine hex
            std::stringstream hex_comb{};
            hex_comb << *next << *nextnext;
            int hex;
            hex_comb >> std::hex >> hex;
            ss << static_cast<char>(hex);
            iter += 2;
        } else {
            ss << c;
        }
    }

    return ss.str();
}

std::string encode_url(std::string_view url_to_encode) {
    return percent_encode(url_to_encode);
}

std::string decode_url(std::string_view url_to_decode) {
    return percent_decode(url_to_decode);
}
};  // namespace blank