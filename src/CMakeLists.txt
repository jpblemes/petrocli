add_library(petrocli_lib
    cli.cpp
    csv.cpp
    commands.cpp
    filter.cpp
    rank.cpp
    stats.cpp
    validation.cpp
)

target_include_directories(petrocli_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(petrocli main.cpp)
target_link_libraries(petrocli PRIVATE petrocli_lib)
