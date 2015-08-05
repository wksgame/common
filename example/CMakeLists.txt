include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(../3rdparty)

link_directories(${LIBRARY_OUTPUT_PATH})

message("example link path" ${LIBRARY_OUTPUT_PATH})

add_executable(testserver testserver.cpp)
add_executable(testclient testclient.cpp)
add_executable(testnowtime testNowTime.cpp)
add_executable(testxml testxml.cpp)

target_link_libraries(testserver kiss sqlite dl)
target_link_libraries(testclient kiss)
target_link_libraries(testnowtime kiss)
target_link_libraries(testxml kiss tinyxml)
