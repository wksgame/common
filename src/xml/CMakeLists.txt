include_directories(${PROJECT_SOURCE_DIR}/src/3rdparty/tinyxml)
include_directories(${PROJECT_SOURCE_DIR}/src)

set(XML_DIR  XMLParser.cpp
)

add_library(kissxml ${XML_DIR})

target_link_libraries(kissxml libtinyxml)