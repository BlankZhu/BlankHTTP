#pragma once

#include <memory>
#include <vector>

#include "Context.h"
#include "DefaultMiddleware.h"
#include "Handler.h"
#include "Middleware.h"
#include "Response.h"

namespace blank
{
    class HandleChain : public Handler
    {
    public:
        HandleChain();
        HandleChain(HandlerPtr handler, bool use_default = true);
        HandleChain(std::vector<MiddlewarePtr> middlewares, HandlerPtr handler, bool use_default = true);
        virtual ~HandleChain() = default;

    public:
        virtual Response handle_request(ContextPtr context, Request &&request);

    private:
        HandlerPtr handler_;
        std::vector<MiddlewarePtr> middlewares_;
    };
    using HandleChainPtr = std::shared_ptr<HandleChain>;
};