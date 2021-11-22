set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.hpp *.cpp)

set(SOURCES ${SOURCES})
# -set(FILES
# -       src/networking/Server.cpp
# -       src/networking/Server.hpp
# -       src/networking/Webserver.cpp
# -       src/networking/Webserver.hpp
# -       src/parsing/ConfigServer.cpp
# -       src/parsing/ConfigServer.hpp
# -       src/parsing/parser.cpp
# -       src/parsing/parser.hpp
# -       main.cpp)
# -add_executable(webserv ${FILES})
# -set_property(TARGET webserv PROPERTY CXX_STANDARD 98)

add_executable(${BINARY}_run ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})