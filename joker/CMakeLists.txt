cmake_minimum_required(VERSION 3.16.3)

project(joker)

set(EXEC joker)

set(CMAKE_CXX_STANDARD 17)

set( CMAKE_CXX_FLAGS "-O3 -g3 -Wall -c -fmessage-length=0 -I/usr/local/include/GraphicsMagick -pthread -L/usr/local/lib -lGraphicsMagick++ -lGraphicsMagick -ljbig -ltiff -lfreetype -ljpeg -lpng16 -lXext -lSM -lICE -lX11 -llzma -lz -lm -lpthread -lgomp" )

include_directories(src)

file(GLOB SOURCES src/*.cpp)

add_executable(${EXEC} ${SOURCES})
