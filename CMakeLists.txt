cmake_minimum_required(VERSION 2.8)

project(common)

message(${PROJECT_SOURCE_DIR})

add_definitions(-DLOG_OUTPUT)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(3rdparty)
add_subdirectory(src)
add_subdirectory(example)