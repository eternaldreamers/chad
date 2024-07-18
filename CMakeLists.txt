cmake_minimum_required(VERSION 3.27)

project(chad)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSSH REQUIRED libssh)

include_directories(${LIBSSH_INCLUDE_DIRS})
link_directories(${LIBSSH_LIBRARY_DIRS})

set(HEADERS

)

set(SOURCES
    src/chad.cpp
)

add_executable(chad ${SOURCES} ${HEADERS})
target_link_libraries(chad PUBLIC ${LIBSSH_LIBRARIES} pthread)
