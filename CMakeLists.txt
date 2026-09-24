cmake_minimum_required(VERSION 3.20)
project(petrocli VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(PETROCLI_BUILD_TESTS "Build petrocli tests" ON)

add_subdirectory(src)

if(PETROCLI_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Packaging: cpack -G NSIS on Windows, cpack -G DEB on Linux.
set(CPACK_PACKAGE_NAME "petrocli")
set(CPACK_PACKAGE_VENDOR "Joao Paulo Borges Lemes")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Command-line tool for petroleum sample CSV data")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}")

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
elseif(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Joao Paulo Borges Lemes <jpblemes@gmail.com>")
endif()

include(CPack)
