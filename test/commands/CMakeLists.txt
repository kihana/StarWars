cmake_minimum_required(VERSION 3.8)

project(commands_test LANGUAGES CXX)

add_executable(
  ${PROJECT_NAME}
  end_move_test.cpp
  end_rotate_test.cpp
  exception_handler_test.cpp
  move_test.cpp
  repeat_n_test.cpp
  repeat_test.cpp
  rotate_test.cpp
  set_velocity_test.cpp
  start_move_test.cpp
  start_rotate_test.cpp
  utils.h
  write_log_message_test.cpp)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/include/Common.cmake)

find_package(GTest REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE commands GTest::gtest_main)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "test")

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
