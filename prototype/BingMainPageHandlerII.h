#pragma once

#include "BlankHttpConstant.h"
#include "BlankHttpHandler.h"
#include "BlankHttpsClient.hpp"

class BingMainPageHandlerII : public blank::BlankHttpHandler
{
public:
    BingMainPageHandlerII() : blank::BlankHttpHandler() {}
    virtual ~BingMainPageHandlerII() = default;

public:
    virtual blank::BlankHttpResponse handle_request(blank::BlankHttpContextPtr ctx, blank::Request &&req);
};