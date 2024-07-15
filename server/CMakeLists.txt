add_executable(<client-name>Server
  <project-name>.cpp
)

target_include_directories(<project-name>Server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(<project-name>Server PUBLIC <project-name>Shared)

