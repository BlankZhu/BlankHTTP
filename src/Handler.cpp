#include "Handler.h"

namespace blank {
std::any Handler::get_extra_data() const { return extra_data_; }

void Handler::set_extra_data(const std::any &data) { extra_data_ = data; }

Response Handler::handle_request(ContextPtr ctx, Request &&req) {
    return base_not_found(ctx, std::move(req));
}

Response Handler::base_not_found(ContextPtr ctx, Request &&req) {
    auto resp_ptr = std::make_shared<StringResponse>(http::status::not_found,
                                                     req.version());
    resp_ptr->set(http::field::server, constant::k_server_name);
    resp_ptr->set(http::field::content_type, constant::k_mime_txt);
    resp_ptr->keep_alive(req.keep_alive());
    resp_ptr->body() = "You've reached a blank space.";
    Response resp{resp_ptr};

    return resp;
}
};  // namespace blank