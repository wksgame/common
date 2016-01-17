include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/3rdparty)

link_directories(${LIBRARY_OUTPUT_PATH})

#message("examples link path:${LIBRARY_OUTPUT_PATH}")

add_executable(testserver testserver.cpp)
add_executable(testclient testclient.cpp)
add_executable(testnowtime testNowTime.cpp)
add_executable(testxml testxml.cpp)
add_executable(testdatabase testdatabase.cpp)
add_executable(testtable testtable.cpp)

target_link_libraries(testserver ${PROJECT_NAME} sqlite dl)
target_link_libraries(testclient ${PROJECT_NAME})
target_link_libraries(testnowtime ${PROJECT_NAME})
target_link_libraries(testxml ${PROJECT_NAME} tinyxml)
target_link_libraries(testdatabase ${PROJECT_NAME} mysqlclient redis)
