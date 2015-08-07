#add_subdirectory(3rdparty)
#add_subdirectory(db)
#add_subdirectory(game)
#add_subdirectory(io)
#add_subdirectory(logger)
#add_subdirectory(message)
#add_subdirectory(network)
#add_subdirectory(other)
#add_subdirectory(platform)
#add_subdirectory(thread)
#add_subdirectory(xml)

include_directories(.)
include_directories(../3rdparty)

aux_source_directory(io/ IO_DIR)

set(LOGGER_DIR		logger/logger.cpp
)

set(DATABASE_DIR	db/SQLiteInterface.cpp
					db/MysqlInterface.cpp
					db/RedisClient/RedisDB.cpp
					db/RedisClient/RedisDBStrings.cpp
					db/RedisClient/RedisDBStringNumber.cpp
					db/RedisClient/RedisDBStringBinary.cpp
					db/RedisClient/RedisDBSortedSets.cpp
					db/RedisClient/RedisDBHashes.cpp
)

set(MESSAGE_DIR		message/ClientSession.cpp
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