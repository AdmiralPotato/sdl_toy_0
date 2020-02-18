cmake_minimum_required(VERSION 3.7)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

project(sdl_toy_0)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

add_executable(sdl_toy_0 ./main.c)
target_link_libraries(sdl_toy_0 ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})