cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)

project(1C)
include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(1C ${SOURCES})
