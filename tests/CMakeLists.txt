include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(petrocli_tests
    cli_test.cpp
    commands_test.cpp
    csv_test.cpp
    filter_test.cpp
    stats_test.cpp
    validation_test.cpp
)

target_link_libraries(petrocli_tests PRIVATE
    petrocli_lib
    gtest_main
)

target_compile_definitions(petrocli_tests PRIVATE
    PETROCLI_TEST_RESOURCES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/resources"
)

include(GoogleTest)
gtest_discover_tests(petrocli_tests)
