option(LOG_OUTPUT "LOG_OUTPUT" ON)

if(LOG_OUTPUT)
	add_definitions(-DLOG_OUTPUT)
endif(LOG_OUTPUT)

if(WIN32)
else(WIN32)
	add_definitions(-std=c++11)
endif(WIN32)

include_directories(.)
include_directories(../3rdparty)

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
					network/IOSocket.cpp
)

set(OTHER_DIR		other/DateTime.cpp
					other/random.cpp
					other/RingBuffer.cpp
)

set(THREAD_DIR		thread/Thread.cpp
					thread/SocketThread.cpp
					thread/WorkThread.cpp
					thread/EpollThread.cpp
)

set(XML_DIR  		xml/XMLParser.cpp
)

aux_source_directory(game/ GAME_DIR)

add_library(kiss	${GAME_DIR}
					${IO_DIR}
					${LOGGER_DIR}
					${DATABASE_DIR}
					${MESSAGE_DIR}
					${NETWORK_DIR}
					${OTHER_DIR}
					${THREAD_DIR}
					${XML_DIR}
)

if(WIN32)
	target_link_libraries(kiss libprotobuf ws2_32.lib)
else(WIN32)
	target_link_libraries(kiss protobuf pthread)
endif(WIN32)
add_subdirectory(tools)