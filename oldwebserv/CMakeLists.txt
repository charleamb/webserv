cmake_minimum_required(VERSION 3.6)

project(webserv)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")#-Wall -Wextra -Werror -std=c++98")

include_directories(src)
add_subdirectory(src)
add_subdirectory(tester)
add_subdirectory(lib/googletest)

