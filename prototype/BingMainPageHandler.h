#pragma once

#include "BlankHttpClient.hpp"
#include "BlankHttpHandler.h"
#include "BlankHttpConstant.h"

class BingMainPageHandler : public blank::BlankHttpHandler
{
public:
    BingMainPageHandler() : blank::BlankHttpHandler() {}
    virtual ~BingMainPageHandler() = default;

public:
    virtual blank::Response handle_request(blank::BlankHttpContextPtr ctx, blank::Request &&req);
};