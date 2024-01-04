cmake_minimum_required(VERSION 3.8)

include(${CMAKE_SOURCE_DIR}/cmake/scripts/prepare_3rdparty.cmake)

message("Generate and configure solution.")
execute_process(COMMAND cmake ${CMAKE_SOURCE_DIR} -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=${CMAKE_SOURCE_DIR}/build/conan_toolchain.cmake -DCMAKE_POLICY_DEFAULT_CMP0091=NEW WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/build)