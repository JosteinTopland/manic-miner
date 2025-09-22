cmake_minimum_required(VERSION 3.16)
project(manic-miner)

set(CMAKE_CXX_STANDARD 20)

set(SDL2_DIR "C:/Users/Jostein/Projects/SDL2-2.32.10/cmake")
set(SDL2_mixer_DIR "C:/Users/Jostein/Projects/SDL2_mixer-2.8.1/cmake")
#set(SDL3_DIR "C:/Users/Jostein/Projects/SDL3-3.2.22/cmake")

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main SDL2::SDL2 SDL2_mixer::SDL2_mixer)
