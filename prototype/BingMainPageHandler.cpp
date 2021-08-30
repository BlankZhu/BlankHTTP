#include "BingMainPageHandler.h"

blank::Response BingMainPageHandler::handle_request(
    blank::BlankHttpContextPtr ctx,
    blank::Request &&req)
{
    blank::Response response{};
    blank::beast::error_code ec;

    std::string host{"www.bing.com"};
    std::string port{"80"};
    std::chrono::seconds timeout{5};
    blank::Request request{blank::http::verb::get, "/", req.version()};
    request.prepare_payload();

    blank::BlankHttpClient client{host, port, timeout, ctx->get_executor()};
    client.DoRequest(request, response, ctx->get_yield_context(), ec);

    if (ec)
    {
        auto err_resp = blank::Response{blank::http::status::internal_server_error, req.version()};
        err_resp.body() = ec.message();
        return err_resp;
    }
    return response;
}