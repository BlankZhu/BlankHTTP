#include "HelloWorldHandler.h"

blank::Response HelloWorldHandler::handle_get(
    blank::BlankHttpContextPtr ctx, 
    blank::Request &&req)
{
    blank::Response res{blank::http::status::ok, req.version()};
    res.set(blank::http::field::server, blank::constant::k_server_name);
    res.set(blank::http::field::content_type, blank::constant::k_mime_txt);
    res.body() = "hello, world";
    res.keep_alive(req.keep_alive());
    return res;
}