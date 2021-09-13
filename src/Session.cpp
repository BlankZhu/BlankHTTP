#include "Session.h"

namespace blank
{
    void Session::handle_session(Logger &logger, net::yield_context yield)
    {
        beast::error_code ec;

        while (true)
        {
            // read in the request
            stream_.expires_after(timeout_);
            Request req;
            http::async_read(stream_, buffer_, req, yield[ec]);
            if (ec == http::error::end_of_stream)
            {
                break;
            }
            if (ec)
            {
                logger.error(fmt("failed to read HTTP request, detail: [%1%]") % ec.message());
                return;
            }

            // handle request, generate response
            // setup context
            auto remote_endpoint = stream_.socket().remote_endpoint();
            auto target = std::make_shared<RequestTarget>();
            target->parse_from_string(req.target().to_string());
            auto context = std::make_shared<Context>(remote_endpoint, target, req.method(), stream_.socket().get_executor(), std::ref(yield), logger);

            auto handler = router_->get_handler(context);
            auto resp = handler->handle_request(context, std::move(req));

            // write the response
            auto http_conn_closed = true;
            if (resp.is_file_response())
            {
                http_conn_closed = write_file_response(resp, yield, ec);
            }
            else if (resp.is_string_response())
            {
                http_conn_closed = write_string_response(resp, yield, ec);
            }

            if (ec)
            {
                logger.error(fmt("failed to write HTTP response, detail: [%1%]") % ec.message());
                return;
            }
            if (http_conn_closed)
            {
                break;
            }
        }

        stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
    }

    bool Session::write_string_response(Response resp, net::yield_context &yield, beast::error_code &ec)
    {
        auto res = resp.get_string_response();
        if (res == nullptr)
        {
            return false;
        }

        auto http_conn_close = res->need_eof();
        stream_.expires_after(timeout_);
        http::async_write(stream_, *res, yield[ec]);
        return http_conn_close;
    }

    bool Session::write_file_response(Response resp, net::yield_context &yield, beast::error_code &ec)
    {
        auto res = resp.get_file_response();
        if (res == nullptr)
        {
            return false;
        }

        auto http_conn_close = res->need_eof();
        stream_.expires_after(timeout_);
        http::async_write(stream_, *res, yield[ec]);
        return http_conn_close;
    }
};