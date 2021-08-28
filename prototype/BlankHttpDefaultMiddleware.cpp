#include "BlankHttpDefaultMiddleware.h"

namespace blank
{
    Response BlankHttpDefaultMiddleware::handle_request(BlankHttpContextPtr ctx, Request &&req)
    {
        auto start = std::chrono::system_clock::now();
        auto resp = next(ctx, std::move(req));
        resp.prepare_payload();
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<float> upstream_duration = end - start;

        BOOST_LOG_TRIVIAL(info) << "[" << resp.result_int() << "] [" << req.method_string()
                                << "] [" << req.target() << "] ["
                                << upstream_duration.count() << "ms]";

        return resp;
    }
};