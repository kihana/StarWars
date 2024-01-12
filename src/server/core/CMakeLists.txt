cmake_minimum_required(VERSION 3.8)

project(core LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC include/${PROJECT_NAME}/angle.h include/${PROJECT_NAME}/object.h
                                   include/${PROJECT_NAME}/vector.h object.cpp vector.cpp)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/include/Common.cmake)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "server")
