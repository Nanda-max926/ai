
cmake_minimum_required(VERSION 3.20)

project(UniVerseBackend LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Drogon CONFIG REQUIRED)

add_executable(universe_api
    src/main.cc
)

target_link_libraries(universe_api
    PRIVATE Drogon::Drogon
)

target_compile_options(universe_api PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
