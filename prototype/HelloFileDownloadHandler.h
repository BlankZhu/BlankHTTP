#pragma once

#include "BlankHttpHandler.h"
#include "BlankHttpConstant.h"

class HelloFileDownloadHandler : public blank::BlankHttpHandler
{
public:
    HelloFileDownloadHandler() :  BlankHttpHandler() {}
    virtual ~HelloFileDownloadHandler() = default;

private:
    virtual blank::BlankHttpResponse handle_request(blank::BlankHttpContextPtr ctx, blank::Request &&req);
};