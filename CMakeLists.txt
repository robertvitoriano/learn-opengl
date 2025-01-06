cmake_minimum_required(VERSION 3.10)

project(FirstProject)

set(CMAKE_CXX_STANDARD 17)

include_directories(include /usr/include)

set(SRC src/main.cpp src/glad.c)

add_executable(FirstProject ${SRC})

target_link_libraries(FirstProject SDL2 GL dl)
