cmake_minimum_required(VERSION 3.20)
project(petrocli LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(PETROCLI_BUILD_TESTS "Build petrocli tests" ON)

add_subdirectory(src)

if(PETROCLI_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
