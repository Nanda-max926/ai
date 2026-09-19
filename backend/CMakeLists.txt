
cmake_minimum_required(VERSION 3.20)

project(UniVerseBackend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Drogon CONFIG REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(
    SODIUM
    REQUIRED
    IMPORTED_TARGET
    libsodium
)

add_executable(universe_api

    src/main.cc

    src/controllers/AuthController.cc

    src/services/PasswordService.cc

    src/repositories/UserRepository.cc
)

target_include_directories(universe_api
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(universe_api
    PRIVATE
    Drogon::Drogon
    PkgConfig::SODIUM
)

target_compile_options(universe_api PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
