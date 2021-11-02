# cmake requirement
cmake_minimum_required(VERSION 3.10.0)

# project detail
project(blankhttp VERSION 0.1.0 DESCRIPTION "A simple asynchronise HTTP library based on boost")

# specify the c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()

add_subdirectory(src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
