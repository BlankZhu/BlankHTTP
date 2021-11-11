add_compile_options(-DBOOST_LOG_DYN_LINK)

add_library(blankhttp SHARED
    AsyncLogger.cpp
    Config.cpp
    Context.cpp
    DefaultMiddleware.cpp
    HandleChain.cpp
    Handler.cpp
    Middleware.cpp
    RequestTarget.cpp
    Response.cpp
    Router.cpp
    Server.cpp
    Session.cpp
    SyncLogger.cpp
    Utility.cpp
)

set (BLANK_HTTP_HEADERS
    AsyncLogger.h
    Client.hpp
    ClientSSL.hpp
    Config.h
    Constant.h
    Context.h
    DefaultMiddleware.h
    HandleChain.h
    Handler.h
    LoggerInterface.h
    Middleware.h
    RequestTarget.h
    Response.h
    RouteNode.h
    Router.h
    Server.h
    Session.h
    SyncLogger.h
    Utility.h
)

install(TARGETS blankhttp DESTINATION lib)
install(FILES ${BLANK_HTTP_HEADERS} DESTINATION include/blankhttp)