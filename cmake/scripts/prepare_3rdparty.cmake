cmake_minimum_required(VERSION 3.19)

message("Prepare 3rd parties for solution.")

execute_process(
  COMMAND
    conan install ${CMAKE_SOURCE_DIR}/conan -if=build -of=build --build=missing
    -pr:b=${CMAKE_SOURCE_DIR}/conan/windows_x64_release -pr:h=${CMAKE_SOURCE_DIR}/conan/windows_x64_release
    COMMAND_ERROR_IS_FATAL ANY)
execute_process(
  COMMAND
    conan install ${CMAKE_SOURCE_DIR}/conan -if=build -of=build --build=missing
    -pr:b=${CMAKE_SOURCE_DIR}/conan/windows_x64_debug -pr:h=${CMAKE_SOURCE_DIR}/conan/windows_x64_debug
    COMMAND_ERROR_IS_FATAL ANY)
