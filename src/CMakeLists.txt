add_library(blankhttp SHARED
    AsyncLogger.cpp
    Client.ipp
    ClientSSL.ipp
    Config.cpp
    Context.cpp
    CoreLogger.cpp
    DefaultMiddleware.cpp
    HandleChain.cpp
    Handler.cpp
    Logger.cpp
    Middleware.cpp
    RequestTarget.cpp
    Response.cpp
    Router.cpp
    RouteTable.cpp
    Server.cpp
    Session.cpp
    SessionSSL.cpp
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
    CoreLogger.h
    DefaultMiddleware.h
    HandleChain.h
    Handler.h
    Logger.h
    LoggerInterface.h
    Middleware.h
    RequestTarget.h
    Response.h
    Router.h
    RouteTable.h
    Server.h
    Session.h
    SessionSSL.h
    SyncLogger.h
    Utility.h
)

install(TARGETS blankhttp DESTINATION lib)
install(FILES ${BLANK_HTTP_HEADERS} DESTINATION include/blankhttp)