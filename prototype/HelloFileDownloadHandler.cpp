#include "HelloFileDownloadHandler.h"

blank::BlankHttpResponse HelloFileDownloadHandler::handle_request(
    blank::BlankHttpContextPtr ctx,
    blank::Request &&req)
{
    using namespace blank;

    std::string hello_file_path{"./hello"};
    beast::error_code error_code;
    http::file_body::value_type body;
    body.open(hello_file_path.c_str(), beast::file_mode::scan, error_code);

    if (error_code == beast::errc::no_such_file_or_directory)
    {
        auto ret = std::make_shared<StringResponse>(http::status::not_found, req.version());
        return BlankHttpResponse{ret};
    }

    auto ret = std::make_shared<FileResponse>(
        std::piecewise_construct,
        std::make_tuple(std::move(body)),
        std::make_tuple(http::status::ok, req.version()));
    ret->set(http::field::server, constant::k_server_name);
    ret->set(http::field::content_type, constant::k_mime_txt);
    ret->content_length(body.size());
    ret->keep_alive(req.keep_alive());
    return BlankHttpResponse{ret};
}