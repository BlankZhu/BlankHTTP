#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace boat {

using Request = boost::beast::http::request<boost::beast::http::string_body>;

};