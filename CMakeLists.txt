cmake_minimum_required(VERSION 2.8)

project(common)

message(${PROJECT_SOURCE_DIR})

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-std=c++11")
endif(UNIX)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_PATH}/cmake)

add_subdirectory(3rdparty)
add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(tools)