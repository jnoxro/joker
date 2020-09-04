cmake_minimum_required(VERSION 3.16.3)

project(joker)

set(EXEC joker)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

file(GLOB SOURCES src/*.cpp)

add_executable(${EXEC} ${SOURCES})
