add_executable(<client-name>Client
  <project-name>.cpp
)

target_include_directories(<project-name>Client PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(<project-name>Client PUBLIC <project-name>Shared)

