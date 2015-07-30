SET(TINY_XML_DIR    tinystr.cpp
                    tinyxml.cpp
                    tinyxmlerror.cpp
                    tinyxmlparser.cpp
)

add_library(tinyxml ${TINY_XML_DIR})
