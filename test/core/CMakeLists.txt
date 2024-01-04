cmake_minimum_required (VERSION 3.8)

project(core_test LANGUAGES CXX)

find_package(GTest REQUIRED)

add_executable (${PROJECT_NAME} "object_test.cpp" "vector_test.cpp")

if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE core GTest::gtest_main)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "test")

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
