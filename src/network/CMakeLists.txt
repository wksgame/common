include_directories(${PROJECT_SOURCE_DIR}/src)

set(NET_WORK_DIR	Socket.cpp)

add_library(kissnetwork ${NET_WORK_DIR})

if(WIN32)
target_link_libraries(kissnetwork ws2_32.lib)
endif(WIN32)