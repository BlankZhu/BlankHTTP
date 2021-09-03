#pragma once

#include "BlankHttpClient.hpp"
#include "BlankHttpConstant.h"
#include "BlankHttpHandler.h"

class BingMainPageHandler : public blank::BlankHttpHandler
{
public:
    BingMainPageHandler() : blank::BlankHttpHandler() {}
    virtual ~BingMainPageHandler() = default;

public:
    virtual blank::BlankHttpResponse handle_request(blank::BlankHttpContextPtr ctx, blank::Request &&req);
};