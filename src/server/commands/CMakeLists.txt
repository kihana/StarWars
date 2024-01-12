cmake_minimum_required(VERSION 3.8)

project(commands LANGUAGES CXX)

find_package(Boost REQUIRED)

add_library(
  ${PROJECT_NAME} STATIC
  command.cpp
  end_command.cpp
  end_move.cpp
  end_rotate.cpp
  exception_handler.cpp
  include/${PROJECT_NAME}/end_command.h
  include/${PROJECT_NAME}/end_move.h
  include/${PROJECT_NAME}/end_rotate.h
  include/${PROJECT_NAME}/exception_handler.h
  #include/${PROJECT_NAME}/macro_command.h
  include/${PROJECT_NAME}/move.h
  include/${PROJECT_NAME}/repeat.h
  include/${PROJECT_NAME}/repeat_n.h
  include/${PROJECT_NAME}/rotate.h
  include/${PROJECT_NAME}/set_angle.h
  include/${PROJECT_NAME}/set_velocity.h
  include/${PROJECT_NAME}/start_command.h
  include/${PROJECT_NAME}/start_move.h
  include/${PROJECT_NAME}/start_rotate.h
  include/${PROJECT_NAME}/write_log_message.h
  include/command.h
  #macro_command.cpp
  move.cpp
  repeat.cpp
  repeat_n.cpp
  rotate.cpp
  set_angle.cpp
  set_velocity.cpp
  start_command.cpp
  start_move.cpp
  start_rotate.cpp
  write_log_message.cpp)

if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC core)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include
  PUBLIC ${Boost_INCLUDE_DIRS}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "server")
