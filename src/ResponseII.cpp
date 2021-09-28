#include "ResponseII.h"
#include <boost/optional/optional_fwd.hpp>

namespace blank {
void ResponseII::set_string_response(StringResponse&& response) {
    string_response_.emplace(response);
    file_response_.reset();
}

void ResponseII::set_file_response(FileResponse&& response) {
    file_response_.emplace(response);
    string_response_.reset();
}

boost::optional<StringResponse> &ResponseII::get_string_response_ref() {
    return string_response_;
}

boost::optional<FileResponse> &ResponseII::get_file_response_ref() {
    return file_response_;
}

bool ResponseII::is_string_response() {
    return string_response_.has_value();
}

bool ResponseII::is_file_response() {
    return file_response_.has_value();
}

boost::optional<StringSerializer>&& ResponseII::get_string_serializer() {
    string_response_->prepare_payload();
    string_serializer_.emplace(*string_response_);
    return std::move(string_serializer_);
}

boost::optional<FileSerializer>&& ResponseII::get_file_serializer() {
    file_response_->prepare_payload();
    file_serializer_.emplace(*file_response_);
    return std::move(file_serializer_);
}
}