cmake_minimum_required(VERSION 3.19)

include(${CMAKE_SOURCE_DIR}/cmake/scripts/configure.cmake)

message("Build solution.")
execute_process(COMMAND cmake --build ${CMAKE_SOURCE_DIR}/build --config Release
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build COMMAND_ERROR_IS_FATAL ANY)
execute_process(COMMAND cmake --build ${CMAKE_SOURCE_DIR}/build --config Debug
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build COMMAND_ERROR_IS_FATAL ANY)
