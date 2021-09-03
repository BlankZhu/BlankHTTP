#include "BingMainPageHandlerII.h"

blank::BlankHttpResponse BingMainPageHandlerII::handle_request(
    blank::BlankHttpContextPtr ctx,
    blank::Request &&req)
{
    using namespace blank;

    auto response = std::make_shared<StringResponse>();
    beast::error_code ec;

    std::string host{"www.bing.com"};
    std::string port{"443"};
    std::chrono::seconds timeout{5};
    ssl::context ssl_context{ssl::context::tlsv12_client};
    ssl_context.set_default_verify_paths();
    ssl_context.set_verify_mode(ssl::verify_peer);
    Request request{http::verb::get, "/", req.version()};
    request.prepare_payload();

    BlankHttpsClient client{host, port, timeout, ssl_context, ctx->get_executor(), ctx->get_yield_context()};
    
    client.DoRequest(request, *response, ec);
    if (ec)
    {
        auto err_resp = std::make_shared<StringResponse>(http::status::internal_server_error, req.version());
        err_resp->body() = ec.message();
        return BlankHttpResponse{err_resp};
    }
    return BlankHttpResponse{response};
}