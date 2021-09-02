#include "BlankHttpHandler.h"

namespace blank
{
    BlankHttpResponse BlankHttpHandler::handle_request(BlankHttpContextPtr ctx, Request &&req)
    {
        return base_not_found(ctx, std::move(req));
    }

    BlankHttpResponse BlankHttpHandler::base_not_found(BlankHttpContextPtr ctx, Request &&req)
    {
        auto resp_ptr = std::make_shared<StringResponse>(http::status::not_found, req.version());
        resp_ptr->set(http::field::server, constant::k_server_name);
        resp_ptr->set(http::field::content_type, constant::k_mime_txt);
        resp_ptr->keep_alive(req.keep_alive());
        resp_ptr->body() = "You've reached a blank space.";
        BlankHttpResponse resp{resp_ptr};

        return resp;
    }
};