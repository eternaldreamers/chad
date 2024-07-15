add_library(<project-name>Shared
  <project-name>.cpp
)

target_include_directories(<project-name>Shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
