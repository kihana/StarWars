cmake_minimum_required(VERSION 3.8)

message("Prepare 3rd parties for solution.")

execute_process(COMMAND conan install ${CMAKE_SOURCE_DIR}/conan -if=build -of=build --build=missing -pr:b=${CMAKE_SOURCE_DIR}/conan/windows_x64_release -pr:h=${CMAKE_SOURCE_DIR}/conan/windows_x64_release)
execute_process(COMMAND conan install ${CMAKE_SOURCE_DIR}/conan -if=build -of=build --build=missing -pr:b=${CMAKE_SOURCE_DIR}/conan/windows_x64_debug -pr:h=${CMAKE_SOURCE_DIR}/conan/windows_x64_debug)