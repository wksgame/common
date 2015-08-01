include_directories(${PROJECT_SOURCE_DIR}/src)
link_directories(${LIBRARY_OUTPUT_PATH})

message("example link path" ${LIBRARY_OUTPUT_PATH})

add_executable(testserver testserver.cpp)
add_executable(testclient testclient.cpp)
add_executable(testnowtime testNowTime.cpp)

target_link_libraries(testserver kiss)
target_link_libraries(testclient kiss)
target_link_libraries(testnowtime kiss)
