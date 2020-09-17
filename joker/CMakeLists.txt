cmake_minimum_required(VERSION 3.16.3)
project(joker)





################################################
#Basic setup

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#defaukt to Release build (instead of debug)
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
	message(STATUS "Defaulting to Release build")
endif()
################################################









################################################
#Check locations for GM previous download/install


#set(GM_DEFAULT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/GraphicsMagick-LATEST.tar.gz") #change to .tar.gz or other archive
set(GM_DEFAULT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/External/download/GraphicsMagick-LATEST.tar.gz") #change to .tar.gz or other archive
set(GM_DEFAULT_INSTALL "${CMAKE_CURRENT_SOURCE_DIR}/External/install/GM/usr/local/include/GraphicsMagick/Magick++.h")

################################################








################################################
#Check if necessary external files are user provided or if make previously GM donloaded & installed


#Set graphicsmagick source path
if(NOT GM_SOURCE)
	#message(STATUS "Default GraphicsMagick source location: ${GM_DEFAULT_SOURCE}")
	set(GM_SOURCE "${GM_DEFAULT_SOURCE}")
else()
	if (GM_SOURCE)
		get_filename_component(GM_SOURCE ${GM_SOURCE} ABSOLUTE) #maybe broken
	endif()
endif()

#check to see if already locally installed
if (EXISTS "${GM_DEFAULT_INSTALL}")
	message(STATUS "Check for GraphicsMagick install: ${GM_DEFAULT_INSTALL} - Found")
	set(GM_INSTALLED "TRUE")
else()
	message(STATUS "Check for GraphicsMagick install: ${GM_DEFAULT_INSTALL} - Not Found")
	set(GM_INSTALLED "FALSE")
endif()

#check to see if GM sourcefiles present
if (EXISTS "${GM_SOURCE}")
	message(STATUS "Check for GraphicsMagick source files: ${GM_SOURCE} - Found")
	set(GM_PROVIDED "TRUE")
else()
	message(STATUS "Check for GraphicsMagick source files: ${GM_SOURCE} - Not Found")
	set(GM_PROVIDED "FALSE")
endif()


#################################################








#################################################
if(NOT ${GM_INSTALLED}) #only do following step if gm not already locally installed
#################################################
#install graphicsmagcik with correct options


include(ExternalProject)

#Set up commands for GM config
set(GM_CONFIG "${CMAKE_CURRENT_SOURCE_DIR}/External/download/GM/files/./configure")
#set(GM_CONFIG "${CMAKE_CURRENT_BINARY_DIR}/GM/SRC_DIR/./configure")
set(GM_CONFIG_ARGS "--with-quantum-depth=8 --without-dps --without-bzlib --without-jbig --without-webp --without-jp2 --without-lcms2 --without-lzma --without-tiff --without-trio --without-ttf --without-wmf --without-xml --without-zlib --without-zstd --with-x=no --with-zlib=no --without-png")
separate_arguments(GM_CONFIG_ARGS)
list(APPEND GM_CONFIG ${GM_CONFIG_ARGS})
message(STATUS "CONFIG_COMAND: ${GM_CONFIG}")

#Set up commands for GM make/build
set(GM_MAKE "make")
set(GM_MAKE_ARGS "-C${CMAKE_CURRENT_SOURCE_DIR}/External/download/GM/files -j4")
#set(GM_MAKE_ARGS "-C${CMAKE_CURRENT_BINARY_DIR}/GM/SRC_DIR -j4")
separate_arguments(GM_MAKE_ARGS)
list(APPEND GM_MAKE ${GM_MAKE_ARGS})
message(STATUS "MAKE_COMAND: ${GM_MAKE}")

#Set up commands for GM install
set(GM_INSTALL "make")
set(GM_INSTALL_ARGS "DESTDIR=${CMAKE_CURRENT_SOURCE_DIR}/External/install/GM install")
#set(GM_INSTALL_ARGS "DESTDIR=${CMAKE_CURRENT_BINARY_DIR}/GM/install install")
separate_arguments(GM_INSTALL_ARGS)
list(APPEND GM_INSTALL ${GM_INSTALL_ARGS})
message(STATUS "INSTALL_COMAND: ${GM_INSTALL}")

#if source files were found earlier, then unpack and use that, otherwie download from GM ftp site
if(${GM_PROVIDED})
ExternalProject_Add(GraphicsMagick_project
	#PREFIX ${CMAKE_CURRENT_BINARY_DIR}/GM
	PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/External/download/GM
	 
	URL ${GM_SOURCE}
	#SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/GM/SRC_DIR
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External/download/GM/files
	 
	CONFIGURE_COMMAND ${GM_CONFIG}
	 
	BUILD_IN_SOURCE 1
	BUILD_COMMAND ${GM_MAKE}
	 
	#INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/GM/install
	INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External/install/GM
	INSTALL_COMMAND ${GM_INSTALL}
)


else()
ExternalProject_Add(GraphicsMagick_project
	#PREFIX ${CMAKE_CURRENT_BINARY_DIR}/GM
	PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/External/download/GM
	
	#SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/GM/SRC_DIR
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External/download/GM/files
	
	URL ftp://ftp.graphicsmagick.org/pub/GraphicsMagick/GraphicsMagick-LATEST.tar.gz
	#DOWNLOAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}
	DOWNLOAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External/download
	 
	CONFIGURE_COMMAND ${GM_CONFIG}
	 
	BUILD_IN_SOURCE 1
	BUILD_COMMAND ${GM_MAKE}
	 
	#INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/GM/install
	INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External/install/GM
	INSTALL_COMMAND ${GM_INSTALL}
)
endif()
##############################################
else()
message(STATUS "GM install skipped")
endif()
##############################################






##############################################
#Add GM libraries to be linked

ExternalProject_Get_Property(GraphicsMagick_project install_dir)

#Set include locations (.h files)
SET(GM_INCLUDE_DIR ${install_dir}/usr/local/include/GraphicsMagick)
get_filename_component(GM_INCLUDE_DIR ${GM_INCLUDE_DIR} ABSOLUTE)
include_directories(${GM_INCLUDE_DIR} ${GM_INCLUDE_DIR}/)

#Set lib locations (.a files)
SET(GM_LIB_DIR ${install_dir}/usr/local/lib)
get_filename_component(GM_LIB_DIR ${GM_LIB_DIR} ABSOLUTE)
link_directories(${GM_LIB_DIR} ${GM_LIB_DIR}/)


#Add GM and GM++ with the set directories
add_library(GRAPHICSMAGICK_LIB STATIC IMPORTED)
set_target_properties(GRAPHICSMAGICK_LIB PROPERTIES IMPORTED_LOCATION ${GM_LIB_DIR}/libGraphicsMagick.a)

add_library(GRAPHICSMAGICKXX_LIB STATIC IMPORTED)
set_target_properties(GRAPHICSMAGICKXX_LIB PROPERTIES IMPORTED_LOCATION ${GM_LIB_DIR}/libGraphicsMagick++.a)

set(LIBRARIES ${GRAPHICSMAGICK_LIB} ${GRAPHICSMAGICKXX_LIB})
###################################################









#################################################
#Add joker executable and link libs

#set sources
file(GLOB SOURCES src/*.cpp) #GLOB not recomended -> add sources individually
add_executable(joker ${SOURCES})


add_dependencies(joker GraphicsMagick_project) #builds graphicsmagick
include_directories(src)

target_link_libraries(joker GRAPHICSMAGICK_LIB GRAPHICSMAGICKXX_LIB GraphicsMagick GraphicsMagick++ jpeg m pthread gomp)
#target_link_libraries(joker ${LIBRARIES} GraphicsMagick GraphicsMagick++ jbig tiff freetype jpeg png16 Xext SM ICE X11 lzma z m pthread gomp)

##################################################




