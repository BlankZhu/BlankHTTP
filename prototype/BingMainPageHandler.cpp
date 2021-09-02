#include "BingMainPageHandler.h"

blank::BlankHttpResponse BingMainPageHandler::handle_request(
    blank::BlankHttpContextPtr ctx,
    blank::Request &&req)
{
    using namespace blank;

    auto response = std::make_shared<StringResponse>();
    beast::error_code ec;

    std::string host{"www.bing.com"};
    std::string port{"80"};
    std::chrono::seconds timeout{5};
    Request request{http::verb::get, "/", req.version()};
    request.prepare_payload();

    BlankHttpClient client{host, port, timeout, ctx->get_executor()};
    client.DoRequest(request, *response, ctx->get_yield_context(), ec);

    if (ec)
    {
        auto err_resp = std::make_shared<StringResponse>(http::status::internal_server_error, req.version());
        err_resp->body() = ec.message();
        return BlankHttpResponse{err_resp};
    }
    return BlankHttpResponse{response};
}