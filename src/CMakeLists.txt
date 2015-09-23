option(LOG_OUTPUT "LOG_OUTPUT" ON)
if(LOG_OUTPUT)
	add_definitions(-DLOG_OUTPUT)
endif(LOG_OUTPUT)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/3rdparty)

aux_source_directory(io/ IO_DIR)

set(LOGGER_DIR		logger/logger.cpp
)

set(DATABASE_DIR	db/SQLiteInterface.cpp
					db/MysqlInterface.cpp
					db/RedisDB.cpp
)

set(MESSAGE_DIR		message/Session.cpp
					message/ClientSession.cpp
					message/msg.pb.cc
)

set(NETWORK_DIR		network/Socket.cpp
					network/TCPSocket.cpp
					network/ServerSocket.cpp
					network/TCPIOSocket.cpp
					network/ClientSocket.cpp
					network/AcceptSocket.cpp
					network/EpollManage.cpp
					network/SelectManage.cpp
)

set(OTHER_DIR		other/DateTime.cpp
					other/random.cpp
					other/RingBuffer.cpp
)

set(THREAD_DIR		thread/Thread.cpp
					thread/WorkThread.cpp
					thread/SelectThread.cpp
					thread/EpollThread.cpp
)

set(XML_DIR  		xml/XMLParser.cpp
)

aux_source_directory(game/ GAME_DIR)

add_library(${PROJECT_NAME} STATIC
			${GAME_DIR}
			${IO_DIR}
			${LOGGER_DIR}
			${DATABASE_DIR}
			${MESSAGE_DIR}
			${NETWORK_DIR}
			${OTHER_DIR}
			${THREAD_DIR}
			${XML_DIR}
)

#set(CMAKE_LINK_LIBRARY_SUFFIX ".a")

if(WIN32)
	target_link_libraries(${PROJECT_NAME} libprotobuf ws2_32.lib)
endif(WIN32)

if(UNIX)
	target_link_libraries(${PROJECT_NAME} protobuf pthread)
endif(UNIX)