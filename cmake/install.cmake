# install CMake package config files

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
)
install(EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${ConfigPackageLocation}
)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  DESTINATION ${ConfigPackageLocation}
)

# export from the build-tree
export(EXPORT ${PROJECT_NAME}Targets
  NAMESPACE ${PROJECT_NAME}::
  FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake
)
