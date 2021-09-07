include(CheckCXXCompilerFlag)

option(BUILD_SHARED_LIBS OFF "Build shared libraries by default")
option(BUILD_STATIC_EXECS  OFF "Link executables statically")
if (BUILD_STATIC_EXECS)
  set(BUILD_SHARED_LIBS OFF FORCE)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
  set(LINK_SEARCH_START_STATIC TRUE)
  set(LINK_SEARCH_END_STATIC TRUE)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release"
  CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# SENSEI should always be position independent
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# SENSEI requires minimum C++11
if (NOT CMAKE_CXX_STANDARD OR CMAKE_CXX_STANDARD LESS 11)
  set(CMAKE_CXX_STANDARD 11)
endif ()

if (NOT MSVC)
  string(APPEND CMAKE_CXX_FLAGS "-Wall -Wextra")

  # Need to explicitly set optimization level to -O3 because some systems default to -O2
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
  set(optimization_flags " -mtune=native; -march=native")
  foreach (flag ${optimization_flags})
    check_cxx_compiler_flag(${flag} has_flag)
    if(has_flag)
      string(APPEND CMAKE_CXX_FLAGS_RELEASE ${flag})
    endif ()
  endforeach ()

  if (BUILD_STATIC_EXECS)
    string(APPEND CMAKE_CXX_FLAGS "-static -static-libgcc -static-libstdc++ -pthread -Wl,-Bstatic")
  endif ()
endif ()

if (APPLE)
  if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    string(APPEND CMAKE_CXX_FLAGS "-stdlib=libc++")
  endif ()
endif ()

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
message(STATUS "BUILD_SHARED_LIBS=${BUILD_SHARED_LIBS}")
message(STATUS "BUILD_STATIC_EXECS=${BUILD_STATIC_EXECS}")
