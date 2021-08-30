#include "BlankHttpSession.h"

namespace blank
{
    void BlankHttpSession::handle_session(net::yield_context yield)
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
                BOOST_LOG_TRIVIAL(error) << "HTTP session read request error, detail: " << ec.message();
                return;
            }

            // handle request, generate response
            // setup context
            auto remote_endpoint = stream_.socket().remote_endpoint();
            auto target = std::make_shared<BlankHttpRequestTarget>();
            target->parse_from_string(req.target().to_string());
            auto context = std::make_shared<BlankHttpContext>(remote_endpoint, target, stream_.socket().get_executor(), std::ref(yield));

            auto handler = router_->get_handler(context);
            auto resp = handler->handle_request(context, std::move(req));

            // write the response
            auto http_connection_closed = resp.need_eof();
            stream_.expires_after(timeout_);
            http::async_write(stream_, resp, yield[ec]);
            if (ec)
            {
                BOOST_LOG_TRIVIAL(error) << "HTTP session write response error, detail: " << ec.message();
                return;
            }
            if (http_connection_closed)
            {
                break;
            }
        }

        stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
    }
};