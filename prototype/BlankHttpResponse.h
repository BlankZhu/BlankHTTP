#pragma once

#include <memory>

#include <boost/beast.hpp>
#include <boost/beast/http.hpp>

namespace blank
{
    namespace http = boost::beast::http;
    using StringResponse = http::response<http::string_body>;
    using FileResponse = http::response<http::file_body>;
    using StringResponsePtr = std::shared_ptr<http::response<http::string_body>>;
    using FileResponsePtr = std::shared_ptr<http::response<http::file_body>>;

    class BlankHttpResponse
    {
    public:
        BlankHttpResponse(StringResponsePtr resp) : is_string_response_(true), is_file_response_(false), string_response_(resp), file_response_(nullptr) {}
        BlankHttpResponse(FileResponsePtr resp) : is_string_response_(false), is_file_response_(true), string_response_(nullptr), file_response_(resp) {}
        ~BlankHttpResponse() = default;

    public:
        StringResponsePtr get_string_response() const;
        void set_string_response(StringResponsePtr resp);
        FileResponsePtr get_file_response() const;
        void set_file_response(FileResponsePtr resp);
        bool is_string_response() const;
        bool is_file_response() const;
        void prepare_payload();
        unsigned get_status_code() const;

    private:
        bool is_string_response_;
        bool is_file_response_;
        StringResponsePtr string_response_;
        FileResponsePtr file_response_;
    };
};