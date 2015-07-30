SET(SQLITE_DIR      shell.c
                    sqlite3.c
)

add_library(sqlite ${SQLITE_DIR})