add_library(blankhttp SHARED
    BlankHttpContext.cpp
    BlankHttpDefaultMiddleware.cpp
    BlankHttpHandleChain.cpp
    BlankHttpHandler.cpp
    BlankHttpLogger.cpp
    BlankHttpMiddleware.cpp
    BlankHttpRequestTarget.cpp
    BlankHttpResponse.cpp
    BlankHttpRouter.cpp
    BlankHttpRouteTable.cpp
    BlankHttpServer.cpp
    BlankHttpSession.cpp
)

set (BLANK_HTTP_HEADERS
    BlankHttpClient.hpp
    BlankHttpConfig.h
    BlankHttpConstant.h
    BlankHttpContext.h
    BlankHttpDefaultMiddleware.h
    BlankHttpHandleChain.h
    BlankHttpHandler.h
    BlankHttpLogger.h
    BlankHttpMiddleware.h
    BlankHttpRequestTarget.h
    BlankHttpResponse.h
    BlankHttpRouter.h
    BlankHttpRouteTable.h
    BlankHttpsClient.hpp
    BlankHttpServer.h
    BlankHttpSession.h
)

install(TARGETS blankhttp DESTINATION lib)
install(FILES ${BLANK_HTTP_HEADERS} DESTINATION include/blankhttp)