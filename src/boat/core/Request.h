#ifndef BOAT_CORE_REQUEST_H
#define BOAT_CORE_REQUEST_H

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace boat {

using Request = boost::beast::http::request<boost::beast::http::string_body>;

};

#endif