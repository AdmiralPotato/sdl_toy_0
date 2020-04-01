cmake_minimum_required(VERSION 3.17)
project(sdl_toy_0)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_CLANG_TIDY clang-tidy)
set(CMAKE_CXX_CLANG_TIDY clang-tidy)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(ExternalProject)
set(
    TMX_PREFIX
    ${CMAKE_SOURCE_DIR}/dependencies/tmx
)
ExternalProject_Add(
    tmx
    PREFIX ${TMX_PREFIX}
    GIT_REPOSITORY https://github.com/baylej/tmx.git
    GIT_TAG 17988db74e401e60d82d5f6b6985f33cbd84bb96
    INSTALL_COMMAND ""
)
set(
    TMX_INCLUDE_DIRS
    ${TMX_PREFIX}/src/tmx/src
)
set(
    TMX_LIBRARIES
    ${TMX_PREFIX}/src/tmx-build/libtmx.a
)
find_package(LibXml2 REQUIRED) # used by the linker below

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(
    ${TMX_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)

file(GLOB SOURCES
    "./src/*.h"
    "./src/*.c"
)
file(
    COPY
    ${CMAKE_SOURCE_DIR}/src/data/
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/
)

add_executable(
    sdl_toy_0
    ${SOURCES}
)
target_link_libraries(
    sdl_toy_0
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}

    ${TMX_LIBRARIES}
    # add zlib and libxml2 to the linker arguments, because ¯\_(ツ)_/¯
    # I thought that building the dependency before-hand would already have resolved that?
    ${LIBXML2_LIBRARIES}
    z # ...and this is how to add zlib. Sure. Whatever.
)
add_dependencies(
    sdl_toy_0
    tmx
)