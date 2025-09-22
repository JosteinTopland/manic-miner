cmake_minimum_required(VERSION 3.16)
project(manic-miner)

set(CMAKE_CXX_STANDARD 20)

add_executable(manic-miner src/main.cpp)

#set(SDL3_DIR "C:/Users/Jostein/Projects/SDL3-3.2.20/cmake")
find_package(SDL2 REQUIRED CONFIG)
find_package(SDL2_mixer REQUIRED CONFIG)
target_link_libraries(manic-miner PRIVATE SDL2::SDL2 SDL2_mixer::SDL2_mixer)
