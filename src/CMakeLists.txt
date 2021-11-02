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
    DefaultMiddleware.h
    HandleChain.h
    Handler.h
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

# check dependencies
find_package(Boost 1.75.0 REQUIRED COMPONENTS atomic chrono context coroutine log log_setup json thread)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(fmt)
target_link_libraries(blankhttp fmt::fmt)

install(TARGETS blankhttp DESTINATION lib)
install(FILES ${BLANK_HTTP_HEADERS} DESTINATION include/blankhttp)