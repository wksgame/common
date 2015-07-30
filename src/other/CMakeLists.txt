include_directories(${PROJECT_SOURCE_DIR}/src)

set(OTHER_DIR	DateTime.cpp
				random.cpp
				RingBuffer.cpp
)

add_library(kissother ${OTHER_DIR})