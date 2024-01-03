cmake_minimum_required (VERSION 3.8)

project(commands LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC
  "include/command.h"
  "include/${PROJECT_NAME}/end_move.h"
  "include/${PROJECT_NAME}/move.h"
  "include/${PROJECT_NAME}/rotate.h"
  "include/${PROJECT_NAME}/set_velocity.h"
  "include/${PROJECT_NAME}/start_move.h"
  "command.cpp"
  "end_move.cpp"
  "move.cpp"
  "rotate.cpp"
  "set_velocity.cpp"
  "start_move.cpp")

if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC core)

target_include_directories(${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "server")
