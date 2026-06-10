cmake_minimum_required(VERSION 3.24...3.30)
project(Pong)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5          # pin to a specific raylib version
)
FetchContent_MakeAvailable(raylib)

add_executable(pong main.cpp)
target_link_libraries(pong raylib)