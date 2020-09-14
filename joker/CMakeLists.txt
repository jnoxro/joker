cmake_minimum_required(VERSION 3.16.3)
project(joker)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(GM_DEFAULT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/GraphicsMagick-LATEST.tar.gz") #change to .tar.gz or other archive



#set release as default
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
	message(STATUS "Defaulting to Release build")
endif()




#Set graphicsmagick source path
if(NOT (GM_SOURCE OR GM_SOURCE_ABS))
	message(STATUS "Default GraphicsMagick source location: ${GM_DEFAULT_SOURCE}")
	set(GM_SOURCE "${GM_DEFAULT_SOURCE}")
else()
	if (GM_SOURCE)
		get_filename_component(GM_SOURCE ${GM_SOURCE} ABSOLUTE) #maybe broken
	endif()
	if (GM_SOURCE_ABS)
		set(GM_SOURCE "${GM_SOURCE_ABS}")
	endif()
endif()




#check to see if sourcefiles present
if (EXISTS "${GM_SOURCE}")
	message(STATUS "Check for GraphicsMagick source: ${GM_SOURCE} - Found")
	set(GM_PROVIDED "TRUE")
else()
	message(STATUS "Check for GraphicsMagick source: ${GM_SOURCE} - Not Found")
	set(GM_PROVIDED "FALSE")
endif()




#install graphicsmagcik with correct options
include(ExternalProject)

set(GM_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/GM/SRC_DIR/")

set(GM_CONFIG_ARGS "--with-quantum-depth=8 --without-bzlib --without-dps --without-jbig --without-webp --without-jp2 --without-lcms2 --without-lzma --without-tiff --without-trio --without-ttf --without-wmf --without-xml --without-zlib --without-zstd --with-x=no --with-zlib=no --without-png")

if(${GM_PROVIDED})
ExternalProject_Add(GraphicsMagick_project
	 PREFIX ${CMAKE_CURRENT_BINARY_DIR}/GM
	 
	 URL ${GM_SOURCE}
	 SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/GM/SRC_DIR
	 

	 CONFIGURE_COMMAND ${GM_CONFIG}./configure ${GM_CONFIG_ARGS}
	 
	 BUILD_IN_SOURCE 1
	 BUILD_COMMAND /GM/SRC_DIR/make
	 
	 INSTALL_COMMAND /GM/SRC_DIR/make DESTDIR=${CMAKE_CURRENT_BINARY_DIR}/GM/install install
)


else()
ExternalProject_Add(GraphicsMagick_project
	 PREFIX ${CMAKE_CURRENT_BINARY_DIR}/GM
	 
	 URL ftp://ftp.graphicsmagick.org/pub/GraphicsMagick/GraphicsMagick-LATEST.tar.gz
	 DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/GM/SRC_DIR
	 
	 CONFIGURE_COMMAND ${GM_CONFIG}./configure ${GM_CONFIG_ARGS}
	 
	 BUILD_IN_SOURCE 1
	 BUILD_COMMAND /GM/SRC_DIR/make
	 
	 INSTALL_COMMAND /GM/SRC_DIR/make DESTDIR=${CMAKE_CURRENT_BINARY_DIR}/GM/install install
)
endif()

add_library(GRAPHICSMAGICK_LIBRARY STATIC IMPORTED)
add_library(GRAPHICSMAGICKXX_LIBRARY STATIC IMPORTED)

set_property(TARGET GRAPHICSMAGICK_LIBRARY PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/GM/install/usr/local/lib/libGraphicsMagick.a)

set_property(TARGET GRAPHICSMAGICKXX_LIBRARY PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/GM/install/usr/local/lib/libGraphicsMagick++.a)

add_dependencies(GRAPHICSMAGICK_LIBRARY GraphicsMagick_project)
add_dependencies(GRAPHICSMAGICKXX_LIBRARY GraphicsMagick_project)


set(GRAPHICSMAGICK_LIBRARIES ${GRAPHICSMAGICK_LIBRARY} ${GRAPHICSMAGICKXX_LIBRARY})
message(STATUS "libs: ${GRAPHICSMAGICK_LIBRARIES}")

#find_path(GRAPHICSMAGICK_INCLUDE_DIR NAMES magick/api.h PATH_SUFFIXES GraphicsMagick)
set(LIBRARIES ${LIBRARIES} ${GRAPHICSMAGICK_LIBRARIES})
set(GRAPHICSMAGICK_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/GM/install/usr/local/include/GrahpicsMagick)

#include(FindPackageHandleStandardArgs)
#find_package_handle_standard_args(GRAPHICSMAGICK DEFAULT_MSG GRAPHICSMAGICK_LIBRARIES GRAPHICSMAGICK_INCLUDE_DIR)

include_directories(src ${GRAPHICSMAGICK_INCLUDE_DIR})
message(STATUS "include dirs: ${GRAPHICSMAGICK_INCLUDE_DIR}")



set(EXEC joker)

#set sources
file(GLOB SOURCES src/*.cpp) #GLOB not recomended -> add sources individually
add_executable(${EXEC} ${SOURCES})
add_dependencies(joker GraphicsMagick_project)

target_link_libraries(joker ${LIBRARIES} GraphicsMagick GraphicsMagick++ jbig tiff freetype jpeg png16 Xext SM ICE X11 lzma z m pthread gomp)




