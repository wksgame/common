include_directories(${PROJECT_SOURCE_DIR}/src)

aux_source_directory(. IO_DIR)

add_library(kissio ${IO_DIR})