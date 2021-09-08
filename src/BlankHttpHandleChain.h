#pragma once

#include <memory>
#include <vector>

#include "BlankHttpContext.h"
#include "BlankHttpDefaultMiddleware.h"
#include "BlankHttpHandler.h"
#include "BlankHttpMiddleware.h"
#include "BlankHttpResponse.h"

namespace blank
{
    class BlankHttpHandleChain : public BlankHttpHandler
    {
    public:
        BlankHttpHandleChain();
        BlankHttpHandleChain(BlankHttpHandlerPtr handler, bool use_default = true);
        BlankHttpHandleChain(std::vector<BlankHttpMiddlewarePtr> middlewares, BlankHttpHandlerPtr handler, bool use_default = true);
        virtual ~BlankHttpHandleChain() = default;

    public:
        virtual BlankHttpResponse handle_request(BlankHttpContextPtr context, Request &&request);

    private:
        BlankHttpHandlerPtr handler_;
        std::vector<BlankHttpMiddlewarePtr> middlewares_;
    };
    using BlankHttpHandleChainPtr = std::shared_ptr<BlankHttpHandleChain>;
};