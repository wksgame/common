#This file used by other project
#include(common.cmake)

include_directories(${SDK_PATH}/src)
link_directories(${SDK_PATH}/lib)

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-std=c++11")
endif(UNIX)