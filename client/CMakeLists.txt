add_executable(chadClient
  chad.cpp
  content.cpp
  gui.cpp
  tcp_client.cpp
)

target_include_directories(chadClient PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(chadClient PUBLIC chadShared)

