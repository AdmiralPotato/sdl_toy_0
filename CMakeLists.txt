cmake_minimum_required(VERSION 3.5)

project(sdl_toy_0)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_CLANG_TIDY clang-tidy)
set(CMAKE_CXX_CLANG_TIDY clang-tidy)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

file(GLOB SOURCES
    "./src/*.h"
    "./src/*.c"
)
file(
    COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data/
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/
)

add_executable(sdl_toy_0 ${SOURCES})
target_link_libraries(sdl_toy_0 ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
