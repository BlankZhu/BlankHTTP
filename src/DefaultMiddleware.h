#pragma once

#include <chrono>

#include "Handler.h"
#include "Logger.h"
#include "Middleware.h"
#include "Response.h"

namespace blank
{
    class DefaultMiddleware : public Middleware
    {
    public:
        DefaultMiddleware() : Middleware() {}
        virtual ~DefaultMiddleware() = default;

    public:
        virtual Response handle_request(ContextPtr ctx, Request &&req);
    };
};