include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(petrocli_tests
    csv_test.cpp
)

target_link_libraries(petrocli_tests PRIVATE
    petrocli_lib
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(petrocli_tests)
