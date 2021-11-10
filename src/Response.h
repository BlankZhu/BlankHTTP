#pragma once

#include <boost/beast/http.hpp>
#include <boost/beast/http/file_body.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/serializer.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/optional/optional.hpp>
#include <boost/pool/detail/mutex.hpp>

namespace blank {
namespace http = boost::beast::http;

using StringResponse = http::response<http::string_body>;
using FileResponse = http::response<http::file_body>;

class Response {
 public:
  Response() = default;
  Response(StringResponse&& response);
  Response(FileResponse&& response);
  Response(Response&& response);
  Response(Response& response) = delete;
  ~Response() = default;

 public:
  unsigned get_status_code() const;
  // getters & setters
  void set_string_response(StringResponse&& response);
  void set_file_response(FileResponse&& response);
  boost::optional<StringResponse>& get_string_response_ref();
  boost::optional<FileResponse>& get_file_response_ref();
  bool is_string_response();
  bool is_file_response();

 private:
  boost::optional<StringResponse> string_response_;
  boost::optional<FileResponse> file_response_;
};

}  // namespace blank