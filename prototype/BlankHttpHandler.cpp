#include "BlankHttpHandler.h"

namespace blank
{
    Response BlankHttpHandler::handle_request(BlankHttpContextPtr ctx, Request &&req)
    {
        auto method = req.method();
        switch (method)
        {
        case http::verb::delete_:
            return handle_delete(ctx, std::move(req));
        case http::verb::get:
            return handle_get(ctx, std::move(req));
        case http::verb::head:
            return handle_head(ctx, std::move(req));
        case http::verb::post:
            return handle_post(ctx, std::move(req));
        case http::verb::put:
            return handle_put(ctx, std::move(req));
        case http::verb::connect:
            return handle_connect(ctx, std::move(req));
        case http::verb::options:
            return handle_options(ctx, std::move(req));
        case http::verb::trace:
            return handle_trace(ctx, std::move(req));
        default:
            return base_not_found(ctx, std::move(req));
        }
    }

    Response BlankHttpHandler::handle_delete(BlankHttpContextPtr ctx, Request &&req)
    {
        return base_not_found(ctx, std::move(req));
    }

    Response BlankHttpHandler::handle_get(BlankHttpContextPtr ctx, Request &&req)
    {
        return base_not_found(ctx, std::move(req));
    }

    Response BlankHttpHandler::handle_head(BlankHttpContextPtr ctx, Request &&req)
    {
        return base_not_found(ctx, std::move(req));
    }

    Response BlankHttpHandler::handle_post(BlankHttpContextPtr ctx, Request &&req)
    {
        return base_not_found(ctx, std::move(req));
    }

    Response BlankHttpHandler::handle_put(BlankHttpContextPtr ctx, Request &&req)
    {
        return base_not_found(ctx, std::move(req));
    }

    Response BlankHttpHandler::handle_connect(BlankHttpContextPtr ctx, Request &&req)
    {
        return base_not_found(ctx, std::move(req));
    }

    Response BlankHttpHandler::handle_options(BlankHttpContextPtr ctx, Request &&req)
    {
        return base_not_found(ctx, std::move(req));
    }

    Response BlankHttpHandler::handle_trace(BlankHttpContextPtr ctx, Request &&req)
    {
        return base_not_found(ctx, std::move(req));
    }

    Response BlankHttpHandler::base_not_found(BlankHttpContextPtr ctx, Request &&req)
    {
        Response res{http::status::not_found, req.version()};
        res.set(http::field::server, constant::k_server_name);
        res.set(http::field::content_type, constant::k_mime_txt);
        res.keep_alive(req.keep_alive());
        res.prepare_payload();
        return res;
    }
};