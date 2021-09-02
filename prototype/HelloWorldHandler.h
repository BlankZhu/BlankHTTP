#pragma once

#include "BlankHttpConstant.h"
#include "BlankHttpHandler.h"
#include "BlankHttpResponse.h"

class HelloWorldHandler : public blank::BlankHttpHandler
{
public:
    HelloWorldHandler() : blank::BlankHttpHandler() {}
    virtual ~HelloWorldHandler() = default;

public:
    virtual blank::BlankHttpResponse handle_request(blank::BlankHttpContextPtr ctx, blank::Request &&req);
};