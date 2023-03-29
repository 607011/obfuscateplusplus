cmake_minimum_required(VERSION 3.16)

project(obf-demo)

set(CMAKE_CXX_STANDARD 17)

set(OBFUSCATION_KEY 0x13371337)

set(CMAKE_CXX_FLAGS "-Wno-format-security -pthread -DOBFUSCATION_KEY=${OBFUSCATION_KEY}")

if(UNIX AND NOT APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
endif()

set(PLATFORM_DEPENDENT_LIBRARIES, "-lpthread")

set(CMAKE_CXX_FLAGS_DEBUG "-Wpedantic -O0 -g3 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -DNDEBUG -Wno-pragmas")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb")
if (APPLE)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -glldb")
endif()

include(CheckFunctionExists)

check_function_exists(memset_s HAVE_MEMSET_S)
if(HAVE_MEMSET_S)
  add_definitions(-DHAVE_MEMSET_S)
endif()
check_function_exists(memset HAVE_MEMSET)
if(HAVE_MEMSET)
  add_definitions(-DHAVE_MEMSET)
endif()
check_function_exists(explicit_bzero HAVE_EXPLICIT_BZERO)
if(HAVE_EXPLICIT_BZERO)
  add_definitions(-DHAVE_EXPLICIT_BZERO)
endif()

add_custom_command(
  OUTPUT hint.txt.h
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/file2h.py ${CMAKE_CURRENT_SOURCE_DIR}/hint.txt ${OBFUSCATION_KEY}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/hint.txt
  VERBATIM)
add_custom_target(hint_txt DEPENDS hint.txt.h)

add_executable(puzzle
  puzzle.cpp
)

add_dependencies(puzzle hint_txt)

add_executable(obf-demo
  obf-demo.cpp
)

add_executable(obf-demo-opt
obf-demo-optimized.cpp
)
