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


#############################################
#create joker library

add_library(joker STATIC src/imgvect.cpp src/model.cpp src/ocr.cpp src/trainer.cpp)
set_target_properties(joker PROPERTIES PUBLIC_HEADER "src/joker.h")
INSTALL(TARGETS joker
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/joker
)

#############################################


#################################################
#Add joker executable and link libs

set(EXE_NAME jk)

#set sources
add_executable(${EXE_NAME} src/main.cpp)

add_dependencies(${EXE_NAME} joker)

include_directories(src)

target_link_libraries(${EXE_NAME} joker jpeg pthread )

INSTALL(TARGETS ${EXE_NAME}
        DESTINATION bin
)

##################################################




