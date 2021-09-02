#pragma once

#include <iostream>

#include "BlankHttpMiddleware.h"
#include "BlankHttpResponse.h"

class HelloUserRepeatMiddleware : public blank::BlankHttpMiddleware
{
public:
    HelloUserRepeatMiddleware() : BlankHttpMiddleware() {};
    virtual ~HelloUserRepeatMiddleware() = default;

public:
    virtual blank::BlankHttpResponse handle_request(blank::BlankHttpContextPtr ctx, blank::Request &&req);
};