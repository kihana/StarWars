cmake_minimum_required(VERSION 3.8)

project(core_test LANGUAGES CXX)

add_executable(${PROJECT_NAME} "object_test.cpp" "vector_test.cpp")

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/include/Common.cmake)

find_package(GTest REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE core GTest::gtest_main)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "test")

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
