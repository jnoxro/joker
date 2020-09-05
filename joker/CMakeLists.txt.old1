cmake_minimum_required(VERSION 3.16.3)
project(joker)

set(EXEC joker)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES src/*.cpp)
add_executable(${EXEC} ${SOURCES} /usr/local/include/GraphicsMagick/Magick++.h)

set (GET_MAGICK "GraphicsMagick++-config")
set (GET_MAGICK_CPP_ARGS "--cppflags")
set (GET_MAGICK_CXX_ARGS "--cxxflags")
set (GET_MAGICK_LD_ARGS "--ldflags")
set (GET_MAGICK_LIB_ARGS "--libs")

message(STATUS "get magick: ${GET_MAGICK} ${GET_MAGICK_CPP_ARGS}")
execute_process(COMMAND ${GET_MAGICK} ${GET_MAGICK_CPP_ARGS}
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  RESULT_VARIABLE MAGICK_RESULT
  OUTPUT_VARIABLE MAGICK_CPP_INFO)
string(STRIP "${MAGICK_CPP_INFO}" MAGICK_CPP_INFO)
message(STATUS "CPP ${MAGICK_CPP_INFO}")

message(STATUS "get magick: ${GET_MAGICK} ${GET_MAGICK_CXX_ARGS}")
execute_process(COMMAND ${GET_MAGICK} ${GET_MAGICK_CXX_ARGS}
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  RESULT_VARIABLE MAGICK_RESULT
  OUTPUT_VARIABLE MAGICK_CXX_INFO)
string(STRIP "${MAGICK_CXX_INFO}" MAGICK_CXX_INFO)
message(STATUS "CXX ${MAGICK_CXX_INFO}")
  
message(STATUS "get magick: ${GET_MAGICK} ${GET_MAGICK_LD_ARGS}")
execute_process(COMMAND ${GET_MAGICK} ${GET_MAGICK_LD_ARGS}
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  RESULT_VARIABLE MAGICK_RESULT
  OUTPUT_VARIABLE MAGICK_LD_INFO)
string(STRIP "${MAGICK_LD_INFO}" MAGICK_LD_INFO)
message(STATUS "LD ${MAGICK_LD_INFO}")
  
message(STATUS "get magick: ${GET_MAGICK} ${GET_MAGICK_LIB_ARGS}")
execute_process(COMMAND ${GET_MAGICK} ${GET_MAGICK_LIB_ARGS}
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  RESULT_VARIABLE MAGICK_RESULT
  OUTPUT_VARIABLE MAGICK_LIB_INFO)
string(STRIP "${MAGICK_LIB_INFO}" MAGICK_LIB_INFO)
message(STATUS "LIB ${MAGICK_LIB_INFO}")


set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MAGICK_CPP_INFO} ${MAGICK_CXX_INFO}")
set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MAGICK_LD_INFO} ${MAGICK_LIB_INFO}")




include_directories(src)
#include_directories(src /usr/local/include /usr/local/include/GraphicsMagick /usr/local/include/GraphicsMagick/magick /usr/local/include/GraphicsMagick/Magick++)

#link_directories(/usr/local/lib)

#target_link_libraries(joker GraphicsMagick GraphicsMagick++ jbig tiff freetype jpeg png16 Xext SM ICE X11 lzma z m pthread gomp)
#target_compile_options(joker AFTER ${MAGICK_CPP_INFO})
#target_link_options(joker AFTER ${MAGICK_LD_INFO})



