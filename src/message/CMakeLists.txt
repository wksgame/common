include_directories(${PROJECT_SOURCE_DIR}/src)

aux_source_directory(. MESSAGE_DIR)

add_library(kissmessage ${MESSAGE_DIR})

target_link_libraries(kissmessage libprotobuf)