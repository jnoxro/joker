cmake_minimum_required(VERSION 3.16.3)
project(joker)

set(EXEC joker)

set(CMAKE_CXX_STANDARD 17)

#set sources
file(GLOB SOURCES src/*.cpp)
add_executable(${EXEC} ${SOURCES} /usr/local/include/GraphicsMagick/Magick++.h)

#set release as default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
  message(STATUS "Defaulting to Release build")
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_library(GRAPHICSMAGICK_LIBRARY NAMES GraphicsMagick)
find_library(GRAPHICSMAGICKXX_LIBRARY NAMES GraphicsMagick++)
set(GRAPHICSMAGICK_LIBRARIES ${GRAPHICSMAGICK_LIBRARY} ${GRAPHICSMAGICKXX_LIBRARY})
message(STATUS "libs: ${GRAPHICSMAGICK_LIBRARIES}")

find_path(GRAPHICSMAGICK_INCLUDE_DIR NAMES magick/api.h PATH_SUFFIXES GraphicsMagick)
set(LIBRARIES ${LIBRARIES} ${GRAPHICSMAGICK_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GRAPHICSMAGICK DEFAULT_MSG GRAPHICSMAGICK_LIBRARIES GRAPHICSMAGICK_INCLUDE_DIR)

include_directories(src ${GRAPHICSMAGICK_INCLUDE_DIR})
message(STATUS "include dirs: ${GRAPHICSMAGICK_INCLUDE_DIR}")

target_link_libraries(joker ${LIBRARIES} GraphicsMagick GraphicsMagick++ jbig tiff freetype jpeg png16 Xext SM ICE X11 lzma z m pthread gomp)




