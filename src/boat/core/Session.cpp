#include "Session.h"
#include "LoggerInterface.h"
#include "RequestTarget.h"

namespace blank {

void Session::reset_expire_time() {
  if (ssl_context_ != nullptr) {
    beast::get_lowest_layer(*ssl_stream_).expires_after(timeout_);
  } else {
    (*tcp_stream_).expires_after(timeout_);
  }
}

void Session::perform_ssl_handshake(net::yield_context yield,
                                    beast::error_code &ec) {
  reset_expire_time();
  (*ssl_stream_).async_handshake(ssl::stream_base::server, yield[ec]);
}

ContextPtr Session::setup_context(const Request &req, bool enable_ssl,
                                  LoggerInterfacePtr logger,
                                  net::yield_context yield) {
  net::ip::tcp::endpoint remote_endpoint{};
  if (enable_ssl) {
    remote_endpoint =
        beast::get_lowest_layer(*ssl_stream_).socket().remote_endpoint();
  } else {
    remote_endpoint = tcp_stream_->socket().remote_endpoint();
  }

  auto target = std::make_shared<RequestTarget>(req.target());

  net::any_io_executor executor;
  if (enable_ssl) {
    executor = beast::get_lowest_layer(*ssl_stream_).socket().get_executor();
  } else {
    executor = (*tcp_stream_).socket().get_executor();
  }

  auto ret = std::make_shared<Context>(remote_endpoint, target, req.method(),
                                       executor, std::ref(yield), logger);
  return ret;
}

void Session::write_response(Response &resp, int http_version, bool enable_ssl,
                             net::yield_context yield, beast::error_code &ec) {
  if (resp.is_string_response()) {
    auto &r = resp.get_string_response_ref();
    _write_response(*r, http_version, enable_ssl, yield, ec);
    return;
  }
  if (resp.is_file_response()) {
    auto &r = resp.get_file_response_ref();
    _write_response(*r, http_version, enable_ssl, yield, ec);
    return;
  }

  // no response set, use a default one
  StringResponse err_resp{http::status::internal_server_error, http_version};
  http::async_write(*tcp_stream_, err_resp, yield[ec]);
  return;
}

void Session::handle_session(LoggerInterfacePtr &logger,
                             net::yield_context yield) {
  beast::error_code ec;
  const auto enable_ssl = (ssl_context_ != nullptr);

  if (enable_ssl) {
    perform_ssl_handshake(yield, ec);
    if (ec) {
      logger->error(fmt("failed to perform SSL handshake, detail: [%1%]") %
                    ec.message());
      return;
    }
  }

  while (true) {
    reset_expire_time();

    // read in the request
    parser_.emplace(std::piecewise_construct, std::make_tuple(),
                    std::make_tuple()  // std::make_tuple(allocator_))
    );
    parser_->header_limit(request_header_limit_);
    parser_->body_limit(request_body_limit_);
    if (enable_ssl) {
      http::async_read(*ssl_stream_, buffer_, *parser_, yield[ec]);
      if (ec == ssl::error::stream_truncated) {
        break;
      }
    } else {
      http::async_read(*tcp_stream_, buffer_, *parser_, yield[ec]);
    }
    if (ec == http::error::end_of_stream) {
      break;
    }
    if (ec) {
      logger->error(fmt("failed to read HTTP request, detail: [%1%]") %
                    ec.message());
      return;
    }
    Request req{std::move(parser_->get())};

    // handle the request
    auto context = setup_context(req, enable_ssl, logger, yield);
    auto handler = router_->get_handler(context);
    auto resp = handler->handle_request(context, std::move(req));

    // write response
    write_response(resp, req.version(), enable_ssl, yield, ec);
    auto need_eof = resp.need_eof();
    if (ec) {
      logger->error(fmt("failed to write HTTP response, detail: [%1%]") %
                    ec.message());
      return;
    }
    if (need_eof) {
      break;
    }
  }

  if (!enable_ssl) {
    (*tcp_stream_).socket().shutdown(tcp::socket::shutdown_send, ec);
  }
}

}  // namespace blank