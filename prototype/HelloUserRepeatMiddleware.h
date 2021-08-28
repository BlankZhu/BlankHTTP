#pragma once

#include <iostream>

#include "BlankHttpMiddleware.h"

class HelloUserRepeatMiddleware : public blank::BlankHttpMiddleware
{
public:
    HelloUserRepeatMiddleware() : BlankHttpMiddleware() {};
    virtual ~HelloUserRepeatMiddleware() = default;

public:
    virtual blank::Response handle_request(blank::BlankHttpContextPtr ctx, blank::Request &&req);
};