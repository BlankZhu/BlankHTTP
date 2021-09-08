# cmake requirement
cmake_minimum_required(VERSION 3.10.0)

# project detail
project(blankhttp VERSION 0.1.0 DESCRIPTION "A simple asynchronise HTTP library based on boost")

# specify the c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# check dependencies
find_package(Boost 1.75.0 REQUIRED COMPONENTS atomic chrono context coroutine json log log_setup thread)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

include(CTest)
enable_testing()

add_library(blankhttp SHARED
    BlankHttpConfig.cpp
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

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

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

install(TARGETS blankhttp DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES ${BLANK_HTTP_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)