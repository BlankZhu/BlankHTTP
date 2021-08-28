#pragma once

#include "BlankHttpConstant.h"
#include "BlankHttpHandler.h"

class HelloWorldHandler : public blank::BlankHttpHandler
{
public:
    HelloWorldHandler() : blank::BlankHttpHandler() {}
    virtual ~HelloWorldHandler() = default;

public:
    virtual blank::Response handle_get(blank::BlankHttpContextPtr ctx, blank::Request &&req);
};