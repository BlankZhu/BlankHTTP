#pragma once

#include <boost/beast/http.hpp>
#include <boost/beast/http/file_body.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/serializer.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/optional/optional.hpp>
#include <boost/pool/detail/mutex.hpp>
#include <boost/pool/pool.hpp>
#include <boost/pool/poolfwd.hpp>

namespace blank {
namespace http = boost::beast::http;

using StringResponse = http::response<http::string_body>;
using StringSerializer = http::response_serializer<http::string_body>;
using FileResponse = http::response<http::file_body>;
using FileSerializer = http::response_serializer<http::file_body>;

class ResponseII {
   public:
    ResponseII() = default;
    ~ResponseII() = default;

   public:
    void set_string_response(StringResponse&& response);
    void set_file_response(FileResponse&& response);
    boost::optional<StringResponse> &get_string_response_ref();
    boost::optional<FileResponse> &get_file_response_ref();
    bool is_string_response();  //
    bool is_file_response();    // todo: if no response(str/file), construct a internal error, and do error log
    boost::optional<StringSerializer>&& get_string_serializer();
    boost::optional<FileSerializer>&& get_file_serializer();

   private:
    boost::optional<StringResponse> string_response_;
    boost::optional<StringSerializer> string_serializer_;
    boost::optional<FileResponse> file_response_;
    boost::optional<FileSerializer> file_serializer_;
};

};  // namespace blank