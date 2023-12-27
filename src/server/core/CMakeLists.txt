cmake_minimum_required (VERSION 3.8)

project("core" LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC "object.cpp" "include/core/object.h" "vector.cpp" "include/core/vector.h" "include/core/angle.h")

if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "server")

target_include_directories(${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/core
)
