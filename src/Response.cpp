#include "Response.h"
#include <boost/optional/optional_fwd.hpp>

namespace blank {
Response::Response(StringResponse&& response) {
    string_response_.emplace(std::move(response));
}

Response::Response(FileResponse&& response) {
    file_response_.emplace(std::move(response));
}

Response::Response(Response&& response) {
    string_response_ = std::move(response.string_response_);
    file_response_ = std::move(response.file_response_);
    // no need to move serializers
}

unsigned Response::get_status_code() const {
    if (string_response_.has_value()) {
        return string_response_->result_int();
    }
    if (file_response_.has_value()) {
        return file_response_->result_int();
    }
    return static_cast<unsigned>(http::status::internal_server_error);
}

void Response::set_string_response(StringResponse&& response) {
    string_response_.emplace(std::move(response));
    file_response_ = boost::none;
}

void Response::set_file_response(FileResponse&& response) {
    file_response_.emplace(std::move(response));
    string_response_ = boost::none;
}

boost::optional<StringResponse>& Response::get_string_response_ref() {
    return string_response_;
}

boost::optional<FileResponse>& Response::get_file_response_ref() {
    return file_response_;
}

bool Response::is_string_response() { return string_response_.has_value(); }

bool Response::is_file_response() { return file_response_.has_value(); }
}  // namespace blank