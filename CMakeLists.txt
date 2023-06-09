cmake_minimum_required(VERSION 3.16)

project(obf-demo)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_FLAGS "-Wno-format-security")
  set(CMAKE_CXX_FLAGS_DEBUG "-Wpedantic -O0 -g3 -ggdb -glldb -DDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -Wno-pragmas")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_FLAGS "-Wno-format-security")
  set(CMAKE_CXX_FLAGS_DEBUG "-Wpedantic -O0 -g3 -ggdb -DDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -Wno-pragmas")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_FLAGS "/EHc /EHs /GF /D__STDC_WANT_SECURE_LIB__")
  set(CMAKE_CXX_FLAGS_DEBUG "/W4 /Od /DDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG")
else()
  set(CMAKE_CXX_STANDARD 20)
endif()

set(OBFUSCATION_KEY 0x13371337)
add_compile_definitions(OBFUSCATION_KEY=${OBFUSCATION_KEY})

include(CheckFunctionExists)
include(CheckSymbolExists)

check_symbol_exists(SecureZeroMemory "Windows.h" HAVE_SECUREZEROMEMORY)
if(HAVE_SECUREZEROMEMORY)
  add_compile_definitions(HAVE_SECUREZEROMEMORY)
endif()

set(CMAKE_REQUIRED_DEFINITIONS -D__STDC_WANT_LIB_EXT1__=1)
check_function_exists(memset_s HAVE_MEMSET_S)
if(HAVE_MEMSET_S)
  add_compile_definitions(HAVE_MEMSET_S)
endif()
unset(CMAKE_REQUIRED_DEFINITIONS)

check_function_exists(explicit_bzero HAVE_EXPLICIT_BZERO)
if(HAVE_EXPLICIT_BZERO)
  add_compile_definitions(HAVE_EXPLICIT_BZERO)
endif()

check_function_exists(memset_explicit HAVE_MEMSET_EXPLICIT)
if(HAVE_MEMSET_EXPLICIT)
  add_compile_definitions(HAVE_MEMSET_EXPLICIT)
endif()

add_custom_command(
  OUTPUT hint.txt.h
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/tools/file2h.py ${CMAKE_CURRENT_SOURCE_DIR}/src/hint.txt ${OBFUSCATION_KEY}
  DEPENDS src/hint.txt
  VERBATIM)
add_custom_target(hint_txt DEPENDS hint.txt.h)

add_executable(puzzle
  src/puzzle.cpp
)

add_dependencies(puzzle hint_txt)

add_executable(cleartext-demo
  src/cleartext-demo.cpp
)

add_executable(obf-demo
  src/obf-demo.cpp
)

add_executable(obf-demo-opt
  src/obf-demo-optimized.cpp
)

get_directory_property(DirDefs COMPILE_DEFINITIONS)
message(STATUS "Compiler definitions:")
foreach(d ${DirDefs})
    message(STATUS "- " ${d})
endforeach()
