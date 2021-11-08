#include "Session.h"

namespace blank {
void Session::handle_session(LoggerInterfacePtr &logger,
                             net::yield_context yield) {
  beast::error_code ec;

  while (true) {
    // read in the request
    stream_.expires_after(timeout_);

    parser_.emplace(std::piecewise_construct, std::make_tuple(),
                    std::make_tuple()  // std::make_tuple(allocator_))
    );
    parser_->header_limit(request_header_limit_);
    parser_->body_limit(request_body_limit_);
    http::async_read(stream_, buffer_, *parser_, yield[ec]);
    if (ec == http::error::end_of_stream) {
      break;
    }
    if (ec) {
      logger->error(fmt("failed to read HTTP request, detail: [%1%]") %
                    ec.message());
      return;
    }
    Request req{std::move(parser_->get())};

    // handle request, generate response
    // setup context
    auto remote_endpoint = stream_.socket().remote_endpoint();
    auto target = std::make_shared<RequestTarget>(req.target());
    auto context = std::make_shared<Context>(
        remote_endpoint, target, req.method(), stream_.socket().get_executor(),
        std::ref(yield), logger);

    auto handler = router_->get_handler(context);
    auto resp = handler->handle_request(context, std::move(req));

    // write the response
    auto http_conn_closed = true;
    http_conn_closed =
        write_response(std::move(resp), req.version(), yield, ec);
    reset_serializer();
    if (ec) {
      logger->error(fmt("failed to write HTTP response, detail: [%1%]") %
                    ec.message());
      return;
    }
    if (http_conn_closed) {
      break;
    }
  }

  stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
}

bool Session::write_response(Response &&resp, int http_version,
                             net::yield_context &yield, beast::error_code &ec) {
  if (resp.is_string_response()) {
    auto &resp_ref = resp.get_string_response_ref();
    resp_ref->prepare_payload();
    auto http_conn_close = resp_ref->need_eof();
    string_serializer_.emplace(*resp_ref);

    stream_.expires_after(timeout_);
    http::async_write(stream_, *string_serializer_, yield[ec]);
    return http_conn_close;
  }
  if (resp.is_file_response()) {
    auto &resp_ref = resp.get_file_response_ref();
    resp_ref->prepare_payload();
    auto http_conn_close = resp_ref->need_eof();
    file_serializer_.emplace(*resp_ref);

    stream_.expires_after(timeout_);
    http::async_write(stream_, *file_serializer_, yield[ec]);
    return http_conn_close;
  }

  // no response set, use a default one
  StringResponse err_resp{http::status::internal_server_error, http_version};
  http::async_write(stream_, err_resp, yield[ec]);
  return false;
}

void Session::reset_serializer() {
  if (string_serializer_.has_value()) {
    string_serializer_ = boost::none;
  }
  if (file_serializer_.has_value()) {
    file_serializer_ = boost::none;
  }
}
};  // namespace blank