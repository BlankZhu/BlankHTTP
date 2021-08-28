#pragma once

#include <sstream>

#include "BlankHttpHandler.h"
#include "BlankHttpConstant.h"

class HelloUserHandler : public blank::BlankHttpHandler
{
public:
    HelloUserHandler() : BlankHttpHandler() {}
    virtual ~HelloUserHandler() = default;

public:
    virtual blank::Response handle_get(blank::BlankHttpContextPtr ctx, blank::Request &&req);
};