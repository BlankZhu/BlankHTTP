#include "BlankHttpConfig.h"
#include "BlankHttpHandleChain.h"
#include "BlankHttpMiddleware.h"
#include "BlankHttpServer.h"

#include "HelloUserHandler.h"
#include "HelloUserRepeatMiddleware.h"
#include "HelloWorldHandler.h"

int main()
{
    blank::BlankHttpConfig config{};
    config.calibrate();
    blank::BlankHttpServer server{config};

    auto hello_world_handler = std::make_shared<HelloWorldHandler>();
    server.register_handler("/hello", hello_world_handler);

    auto repeator1 = std::make_shared<HelloUserRepeatMiddleware>();
    auto repeator2 = std::make_shared<HelloUserRepeatMiddleware>();
    std::vector<blank::BlankHttpMiddlewarePtr> repeat_middlwares{repeator1, repeator2};
    auto hello_user_handler = std::make_shared<HelloUserHandler>();
    auto repeat_user_chain = std::make_shared<blank::BlankHttpHandleChain>(repeat_middlwares, hello_user_handler);
    server.register_handler("/hello/:user", repeat_user_chain);

    server.run();
}