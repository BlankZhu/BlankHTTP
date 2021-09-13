add_library(blankhttp SHARED
    Config.cpp
    Context.cpp
    DefaultMiddleware.cpp
    HandleChain.cpp
    Handler.cpp
    Client.ipp
    ClientSSL.ipp
    Logger.cpp
    Middleware.cpp
    RequestTarget.cpp
    Response.cpp
    Router.cpp
    RouteTable.cpp
    Server.cpp
    Session.cpp
    SessionSSL.cpp
    Utility.cpp
)

set (BLANK_HTTP_HEADERS
    Config.h
    Constant.h
    Context.h
    DefaultMiddleware.h
    HandleChain.h
    Handler.h
    Client.hpp
    ClientSSL.hpp
    Logger.h
    Middleware.h
    RequestTarget.h
    Response.h
    Router.h
    RouteTable.h
    Server.h
    Session.h
    SessionSSL.h
    Utility.h
)

install(TARGETS blankhttp DESTINATION lib)
install(FILES ${BLANK_HTTP_HEADERS} DESTINATION include/blankhttp)