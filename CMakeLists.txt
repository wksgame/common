cmake_minimum_required(VERSION 2.8)

project(common)

message("${PROJECT_NAME} path:${PROJECT_SOURCE_DIR}")

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-std=c++11")
endif(UNIX)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

message("bin path:${EXECUTABLE_OUTPUT_PATH}")
message("lib path:${LIBRARY_OUTPUT_PATH}")

add_subdirectory(3rdparty)
add_subdirectory(src)
#add_subdirectory(examples)
#add_subdirectory(tools)