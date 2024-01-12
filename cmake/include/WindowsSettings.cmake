cmake_minimum_required(VERSION 3.8)

if(CMAKE_GENERATOR MATCHES "Visual Studio")
  # Reduce the config types to only Debug and Release
  set(CMAKE_CONFIGURATION_TYPES
      "Debug;Release"
      CACHE STRING "" FORCE)

  # Enable Hot Reload for MSVC compilers if supported.
  if(POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
        "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug>:EditAndContinue>,$<$<CONFIG:Debug>:ProgramDatabase>>"
    )
  endif()
endif()
