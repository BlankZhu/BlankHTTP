#include <boat/core/Session.h>
#include <boat/log/LoggerInterface.h>

namespace boat {

void Session::reset_expire_time() {
  if (ssl_context_ != nullptr) {
    boost::beast::get_lowest_layer(*ssl_stream_).expires_after(timeout_);
  } else {
    tcp_stream_->expires_after(timeout_);
  }
}

void Session::perform_ssl_handshake(const boost::asio::yield_context &yield,
                                    boost::beast::error_code &ec) {
  reset_expire_time();
  ssl_stream_->async_handshake(boost::asio::ssl::stream_base::server,
                               yield[ec]);
}

ContextPtr Session::setup_context(const Request &req, const bool enable_ssl,
                                  log::LoggerInterfacePtr logger,
                                  boost::asio::yield_context yield) {
  boost::asio::ip::tcp::endpoint remote_endpoint{};
  if (enable_ssl) {
    remote_endpoint =
        boost::beast::get_lowest_layer(*ssl_stream_).socket().remote_endpoint();
  } else {
    remote_endpoint = tcp_stream_->socket().remote_endpoint();
  }

  boost::url url{req.target()};

  boost::asio::any_io_executor executor;
  if (enable_ssl) {
    executor =
        boost::beast::get_lowest_layer(*ssl_stream_).socket().get_executor();
  } else {
    executor = tcp_stream_->socket().get_executor();
  }

  auto ret = std::make_shared<Context>(remote_endpoint, url, req.method(),
                                       executor, std::ref(yield), logger);
  return ret;
}

void Session::write_response(Response &resp, const unsigned http_version,
                             const bool enable_ssl,
                             const boost::asio::yield_context &yield,
                             boost::beast::error_code &ec) {
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
  const StringResponse err_resp{
      boost::beast::http::status::internal_server_error, http_version};
  boost::beast::http::async_write(*tcp_stream_, err_resp, yield[ec]);
}

void Session::handle_session(const log::LoggerInterfacePtr &logger,
                             const boost::asio::yield_context &yield) {
  boost::beast::error_code ec;
  const auto enable_ssl = (ssl_context_ != nullptr);

  if (enable_ssl) {
    perform_ssl_handshake(yield, ec);
    if (ec) {
      logger->Error(
          boost::format("failed to perform SSL handshake, detail: [%1%]") %
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
      boost::beast::http::async_read(*ssl_stream_, buffer_, *parser_,
                                     yield[ec]);
      if (ec == boost::asio::ssl::error::stream_truncated) {
        break;
      }
    } else {
      boost::beast::http::async_read(*tcp_stream_, buffer_, *parser_,
                                     yield[ec]);
    }
    if (ec == boost::beast::http::error::end_of_stream) {
      break;
    }
    if (ec) {
      logger->Error(
          boost::format("failed to read HTTP request, detail: [%1%]") %
          ec.message());
      return;
    }
    Request req{std::move(parser_->get())};

    // handle the request
    const auto context = setup_context(req, enable_ssl, logger, yield);
    const auto handler = router_->get_handler(context);
    auto resp = handler->handle_request(context, std::move(req));

    // write response
    write_response(resp, static_cast<int>(req.version()), enable_ssl, yield,
                   ec);
    const auto need_eof = resp.need_eof();
    if (ec) {
      logger->Error(
          boost::format("failed to write HTTP response, detail: [%1%]") %
          ec.message());
      return;
    }
    if (need_eof) {
      break;
    }
  }

  if (!enable_ssl) {
    tcp_stream_->socket().shutdown(boost::asio::ip::tcp::socket::shutdown_send,
                                   ec);
  }
}

}  // namespace boat