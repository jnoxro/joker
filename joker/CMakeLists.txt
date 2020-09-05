cmake_minimum_required(VERSION 3.16.3)
project(joker)

set(EXEC joker)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES src/*.cpp)
add_executable(${EXEC} ${SOURCES} /usr/local/include/GraphicsMagick/Magick++.h)

find_library(GRAPHICSMAGICK_LIBRARY NAMES GraphicsMagick)
find_library(GRAPHICSMAGICKXX_LIBRARY NAMES GraphicsMagick++)
set(GRAPHICSMAGICK_LIBRARIES ${GRAPHICSMAGICK_LIBRARY} ${GRAPHICSMAGICKXX_LIBRARY})
find_path(GRAPHICSMAGICK_INCLUDE_DIR NAMES magick/api.h PATH_SUFFIXES GraphicsMagick)
set(LIBRARIES ${LIBRARIES} ${GRAPHICSMAGICK_LIBRARIES})
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GRAPHICSMAGICK DEFAULT_MSG GRAPHICSMAGICK_LIBRARIES GRAPHICSMAGICK_INCLUDE_DIR)

include_directories(${GRAPHICSMAGICK_INCLUDE_DIR})

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MAGICK_CPP_INFO} ${MAGICK_CXX_INFO}")
set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MAGICK_LD_INFO} ${MAGICK_LIB_INFO}")




include_directories(src)
#include_directories(src /usr/local/include /usr/local/include/GraphicsMagick /usr/local/include/GraphicsMagick/magick /usr/local/include/GraphicsMagick/Magick++)

#link_directories(/usr/local/lib)

target_link_libraries(joker ${LIBRARIES} GraphicsMagick GraphicsMagick++ jbig tiff freetype jpeg png16 Xext SM ICE X11 lzma z m pthread gomp)
#target_compile_options(joker AFTER ${MAGICK_CPP_INFO})
#target_link_options(joker AFTER ${MAGICK_LD_INFO})



