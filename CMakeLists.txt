cmake_minimum_required(VERSION 4.0)
project(manic-miner)

set(CMAKE_CXX_STANDARD 20)

add_executable(manic-miner src/main.cpp)

set(SDL3_DIR "C:/Users/Jostein/Projects/SDL3-3.2.20/cmake")
find_package(SDL3 REQUIRED)
target_link_libraries(manic-miner PRIVATE SDL3::SDL3)
