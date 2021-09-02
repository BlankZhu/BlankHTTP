#include "HelloUserRepeatMiddleware.h"

blank::BlankHttpResponse HelloUserRepeatMiddleware::handle_request(
    blank::BlankHttpContextPtr ctx, 
    blank::Request &&req)
{
    auto resp = next(ctx, std::move(req));
    // auto body = resp.body();
    // resp.body() = body + body;

    if (resp.is_string_response())
    {
        auto s_resp = resp.get_string_response();
        if (s_resp != nullptr)
        {
            auto old_body = s_resp->body();
            auto new_body = old_body + old_body;
            s_resp->body() = new_body;
        }
    }

    return resp;
}