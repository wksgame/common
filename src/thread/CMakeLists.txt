include_directories(${PROJECT_SOURCE_DIR}/src)

set(THREAD_DIR	Thread.cpp
				SocketThread.cpp
)

add_library(kissthread ${THREAD_DIR})

target_link_libraries(kissthread kissmessage)