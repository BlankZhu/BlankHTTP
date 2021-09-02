#include "HelloUserHandler.h"

blank::BlankHttpResponse HelloUserHandler::handle_request(
    blank::BlankHttpContextPtr ctx,
    blank::Request &&req)
{
    using namespace blank;

    auto res = std::make_shared<StringResponse>(http::status::ok, req.version());

    res->set(blank::http::field::server, blank::constant::k_server_name);
    res->set(blank::http::field::content_type, blank::constant::k_mime_txt);
    
    std::stringstream ss{};
    ss << "Hello, " << ctx->get_param("user") << "\n";
    res->body() = ss.str();
    
    res->keep_alive(req.keep_alive());
    return BlankHttpResponse{res};
}