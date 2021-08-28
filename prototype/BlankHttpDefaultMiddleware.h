#pragma once

#include <chrono>

#include "BlankHttpHandler.h"
#include "BlankHttpLogger.h"
#include "BlankHttpMiddleware.h"

namespace blank
{
    class BlankHttpDefaultMiddleware : public BlankHttpMiddleware
    {
    public:
        BlankHttpDefaultMiddleware() : BlankHttpMiddleware() {}
        virtual ~BlankHttpDefaultMiddleware() = default;

    public:
        virtual Response handle_request(BlankHttpContextPtr ctx, Request &&req);
    };
};