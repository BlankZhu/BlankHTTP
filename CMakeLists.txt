# cmake requirement
cmake_minimum_required(VERSION 3.20.0)

# project detail
project(boat VERSION 0.1.0 DESCRIPTION "A simple asynchronous HTTP library based on boost asio & beast")

# options
option(BUILD_BOAT "build boat library" ON)
option(BUILD_DEMO "build boat demo" OFF)

# specify the c++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# check dependencies
find_package(Boost 1.84.0 REQUIRED COMPONENTS atomic context coroutine log log_setup json thread url)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

if (BUILD_BOAT)
    include_directories(src)
    add_subdirectory(src)
endif ()

if (BUILD_DEMO)
    add_subdirectory(demo)
endif ()
