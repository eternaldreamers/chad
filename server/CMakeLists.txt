add_executable(chadServer
  chad.cpp
)

target_include_directories(chadServer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(chadServer PUBLIC chadShared)

