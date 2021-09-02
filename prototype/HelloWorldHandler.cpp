#include "HelloWorldHandler.h"

blank::BlankHttpResponse HelloWorldHandler::handle_get(
    blank::BlankHttpContextPtr ctx, 
    blank::Request &&req)
{
    using namespace blank;

    auto res = std::make_shared<StringResponse>(http::status::ok, req.version());
    res->set(blank::http::field::server, blank::constant::k_server_name);
    res->set(blank::http::field::content_type, blank::constant::k_mime_txt);
    res->body() = "hello, world";
    res->keep_alive(req.keep_alive());

    return BlankHttpResponse{res};
}