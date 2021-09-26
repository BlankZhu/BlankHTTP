#include "Client.hpp"

namespace blank {
Client::~Client() {
    if (is_connected_) {
        beast::error_code ec;
        Shutdown(ec);
    }
}

void Client::Connect(net::yield_context &yield, beast::error_code &ec) {
    if (is_connected_) {
        return;
    }

    stream_.expires_after(timeout_);
    auto const results = resolver_.async_resolve(host_, port_, yield[ec]);
    if (ec) {
        return;
    }

    stream_.async_connect(results, yield[ec]);
    if (ec) {
        return;
    }

    is_connected_ = true;
}

void Client::Shutdown(beast::error_code &ec) {
    if (is_connected_) {
        stream_.expires_after(timeout_);
        stream_.socket().shutdown(tcp::socket::shutdown_both, ec);
        is_connected_ = false;
    }
}

template <class Body, class Fields>
void Client::DoRequest(const http::message<true, Body, Fields> &request,
                       http::message<false, Body, Fields> &response,
                       net::yield_context &yield, beast::error_code &ec) {
    if (!is_connected_) {
        Connect(yield, ec);
        if (ec) {
            return;
        }
    }

    stream_.expires_after(timeout_);
    http::async_write(stream_, request, yield[ec]);
    if (ec) {
        return;
    }

    beast::flat_buffer buffer;
    http::async_read(stream_, buffer, response, yield[ec]);
    if (ec) {
        return;
    }
}

template <class Body, class Fields>
void Client::Download(const http::message<true, Body, Fields> &request,
                      const std::string &save_path, net::yield_context &yield,
                      beast::error_code &ec) {
    if (!is_connected_) {
        Connect(yield, ec);
        if (ec) {
            return;
        }
    }

    http::response<http::file_body> response;
    response.body().open(save_path.c_str(), beast::file_mode::write, ec);
    if (ec) {
        return;
    }

    stream_.expires_after(timeout_);
    http::async_write(stream_, request, yield[ec]);
    if (ec) {
        return;
    }

    beast::flat_buffer buffer;
    http::async_read(stream_, buffer, response, yield[ec]);
    if (ec) {
        return;
    }
}
};  // namespace blank