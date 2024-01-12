cmake_minimum_required(VERSION 3.8)

project(server LANGUAGES CXX)

add_executable(${PROJECT_NAME} server.cpp)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/include/Common.cmake)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER ${PROJECT_NAME})

add_subdirectory(core)
add_subdirectory(commands)
