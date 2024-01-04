cmake_minimum_required(VERSION 3.8)

if((NOT EXISTS ${CMAKE_SOURCE_DIR}/build/test/core/Debug/core_test.exe) OR (NOT EXISTS ${CMAKE_SOURCE_DIR}/build/test/commands/Debug/commands_test.exe))
	message(FATAL_ERROR "Solution has not been built.")
endif()

file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}" NATIVE_SOURCE_DIR)

message("Test coverage calculation.")
execute_process(COMMAND "C:/Program Files/OpenCppCoverage/OpenCppCoverage.exe" --sources ${NATIVE_SOURCE_DIR}\\src --excluded_line_regex "\\s*\\}.*" --export_type=binary:${CMAKE_SOURCE_DIR}/build/core.cov -- ${CMAKE_SOURCE_DIR}/build/test/core/Debug/core_test.exe WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
execute_process(COMMAND "C:/Program Files/OpenCppCoverage/OpenCppCoverage.exe" --sources ${NATIVE_SOURCE_DIR}\\src --excluded_line_regex "\\s*\\}.*" --export_type=html:${CMAKE_SOURCE_DIR}/build/TestCoverageReport --input_coverage=${CMAKE_SOURCE_DIR}/build/core.cov -- ${CMAKE_SOURCE_DIR}/build/test/commands/Debug/commands_test.exe WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build)