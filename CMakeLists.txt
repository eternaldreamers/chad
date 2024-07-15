cmake_minimum_required(VERSION 3.27)

project(chad)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(shared)
add_subdirectory(server)
add_subdirectory(client)
