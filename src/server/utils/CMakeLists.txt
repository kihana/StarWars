cmake_minimum_required (VERSION 3.8)

project("utils" LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC "move.cpp" "include/utils/move.h" "rotate.cpp" "include/utils/rotate.h")

if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE core)

target_include_directories(${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/utils
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "server")
