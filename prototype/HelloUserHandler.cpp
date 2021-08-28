#include "HelloUserHandler.h"

blank::Response HelloUserHandler::handle_get(
    blank::BlankHttpContextPtr ctx,
    blank::Request &&req)
{
    blank::Response res{blank::http::status::ok, req.version()};
    res.set(blank::http::field::server, blank::constant::k_server_name);
    res.set(blank::http::field::content_type, blank::constant::k_mime_txt);
    
    std::stringstream ss{};
    ss << "Hello, " << ctx->get_param("user") << "\n";
    res.body() = ss.str();

    res.keep_alive(req.keep_alive());
    res.prepare_payload();
    return res;
}