cmake_minimum_required(VERSION 3.10)

project(Triangle)

set(CMAKE_CXX_STANDARD 17)

include_directories(include /usr/include)

file(GLOB SRC "src/*.cpp" "src/*.c")

add_executable(Triangle ${SRC})

target_link_libraries(Triangle SDL2 GL dl)
