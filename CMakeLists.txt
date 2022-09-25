
#--------------------------------------------------------------------
# Global configuration
#--------------------------------------------------------------------

cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)
project("Raster" C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/release)

set(INCLUDE_DIRS
  ${CMAKE_SOURCE_DIR}/src
)

find_package(OpenGL REQUIRED)

#--------------------------------------------------------------------
# Compile source
#--------------------------------------------------------------------

add_subdirectory(deps/glfw)

file(GLOB_RECURSE RAW_SRC CONFIGURE_DEPENDS
  src/*.h
  src/*.c
  shaders/*.glsl
)

add_executable(${PROJECT_NAME}
  ${RAW_SRC}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE
  ${OPENGL_gl_LIBRARY}
  glfw
)

#--------------------------------------------------------------------
# resource files
#--------------------------------------------------------------------

if(NOT "${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")

  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
      "${CMAKE_SOURCE_DIR}/shaders"
      "${CMAKE_BINARY_DIR}/shaders"
    COMMAND ${CMAKE_COMMAND} -E create_symlink
      "${CMAKE_SOURCE_DIR}/data"
      "${CMAKE_BINARY_DIR}/data"
  )

endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/data"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/data"
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/data"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/data"
)

add_custom_target(shaders ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory 
    "${CMAKE_SOURCE_DIR}/shaders"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/shaders"
  COMMAND ${CMAKE_COMMAND} -E copy_directory 
    "${CMAKE_SOURCE_DIR}/shaders"
    "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/shaders"
  COMMENT "Copying resources"
)

add_dependencies(${PROJECT_NAME} shaders)