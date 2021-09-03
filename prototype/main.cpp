#include "BlankHttpConfig.h"
#include "BlankHttpHandleChain.h"
#include "BlankHttpMiddleware.h"
#include "BlankHttpServer.h"

#include "BingMainPageHandler.h"
#include "BingMainPageHandlerII.h"
#include "HelloUserHandler.h"
#include "HelloUserRepeatMiddleware.h"
#include "HelloWorldHandler.h"
#include "HelloFileDownloadHandler.h"

int main()
{
    blank::BlankHttpConfig config{};
    config.calibrate();
    blank::BlankHttpServer server{config};

    auto hello_world_handler = std::make_shared<HelloWorldHandler>();
    server.register_handler("/hello", blank::http::verb::get, hello_world_handler);

    auto bing_main_page_handler = std::make_shared<BingMainPageHandler>();
    server.register_handler("/bing", blank::http::verb::get, bing_main_page_handler);

    auto bing_main_page_hander_ii = std::make_shared<BingMainPageHandlerII>();
    server.register_handler("/ssl/bing", blank::http::verb::get, bing_main_page_hander_ii);

    auto repeator = std::make_shared<HelloUserRepeatMiddleware>();
    std::vector<blank::BlankHttpMiddlewarePtr> repeat_middlwares{repeator};
    auto hello_user_handler = std::make_shared<HelloUserHandler>();
    auto repeat_user_chain = std::make_shared<blank::BlankHttpHandleChain>(repeat_middlwares, hello_user_handler);
    server.register_chain("/hello/:user", blank::http::verb::get, repeat_user_chain);

    auto hello_file_handler = std::make_shared<HelloFileDownloadHandler>();
    server.register_handler("/download/hello", blank::http::verb::get, hello_file_handler);

    server.run();
}