#include "HelloUserRepeatMiddleware.h"

blank::Response HelloUserRepeatMiddleware::handle_request(
    blank::BlankHttpContextPtr ctx, 
    blank::Request &&req)
{
    auto resp = next(ctx, std::move(req));
    auto body = resp.body();
    resp.body() = body + body;
    return resp;
}