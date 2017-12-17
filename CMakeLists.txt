cmake_minimum_required(VERSION 2.6)

# project name is set to aurora
project(aurora)

set(aurora_SRCS src/aurora.cpp src/shader.cpp src/glad.c)

add_definitions(-std=c++11)

include_directories(include)
link_libraries(glfw dl)

add_executable(aurora ${aurora_SRCS})
