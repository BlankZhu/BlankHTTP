#ifndef BOAT_CORE_RESPONSE_H
#define BOAT_CORE_RESPONSE_H

#include <optional>

#include <boost/beast/http.hpp>
#include <boost/beast/http/file_body.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>

namespace boat {

using StringResponse =
    boost::beast::http::response<boost::beast::http::string_body>;
using FileResponse =
    boost::beast::http::response<boost::beast::http::file_body>;

class Response {
 public:
  Response() = default;
  explicit Response(StringResponse&& response);
  explicit Response(FileResponse&& response);
  Response(Response&& response) noexcept;
  Response(Response& response) = delete;
  ~Response() = default;

 public:
  [[nodiscard]] unsigned get_status_code() const;
  // getters & setters
  void set_string_response(StringResponse&& response);
  void set_file_response(FileResponse&& response);
  std::optional<StringResponse>& get_string_response_ref();
  std::optional<FileResponse>& get_file_response_ref();
  [[nodiscard]] bool is_string_response() const;
  [[nodiscard]] bool is_file_response() const;
  [[nodiscard]] bool need_eof() const;

 private:
  std::optional<StringResponse> string_response_;
  std::optional<FileResponse> file_response_;
};

}  // namespace boat

#endif