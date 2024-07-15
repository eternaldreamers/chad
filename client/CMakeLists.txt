add_executable(chadClient
  chad.cpp
)

target_include_directories(chadClient PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(chadClient PUBLIC chadShared)

