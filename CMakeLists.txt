cmake_minimum_required(VERSION 3.16)
project(lazyfoo_sdl2)

set(CMAKE_CXX_STANDARD 17)

# SDL2 stuff
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_gfx REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} SDL2::Main SDL2::Image SDL2::Mixer SDL2::TTF SDL2::GFX)
