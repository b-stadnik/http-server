cmake_minimum_required(VERSION 3.12)

project(http_server)

option(ENABLE_SANITIZERS "Enable sanitizers" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wno-unused-parameter")

if(ENABLE_SANITIZERS)
  set(sanitizer_options "-fsanitize=address,undefined,return")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${sanitizer_options}")
endif()

# Enable cppcheck
find_program(CPPCHECK cppcheck)
if(CPPCHECK)
  set(CMAKE_CXX_CPPCHECK
      "${CPPCHECK}"
      "--quiet"
      "--inline-suppr"
      "--enable=warning,style,performance,portability"
      "--std=c++17"
      "--suppress=unusedFunction"
      "--suppress=*:*external\*"
      "--template={file}:{line}:({severity}):{id}: {message}")
else()
  message(WARNING "cppcheck not found, static analysis will be skipped.")
endif()

# Enable clang-tidy
find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
  set(CLANG_TIDY_CHECKS
      *
      # add checks to ignore here:
      -modernize-use-trailing-return-type
      -bugprone-assignment-in-if-condition
      -bugprone-easily-swappable-parameters
      -bugprone-implicit-widening-of-multiplication-result
      -bugprone-macro-parentheses
      -bugprone-narrowing-conversions
      -bugprone-reserved-identifier
      -bugprone-sizeof-expression
      -cert-dcl37-c
      -cert-dcl51-cpp
      -cert-err33-c
      -clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling
      -concurrency-mt-unsafe
      -cppcoreguidelines-avoid-magic-numbers
      -cppcoreguidelines-avoid-non-const-global-variables
      -cppcoreguidelines-init-variables
      -cppcoreguidelines-narrowing-conversions
      -google-readability-function-size
      -google-readability-todo
      -hicpp-function-size
      -hicpp-no-assembler
      -hicpp-signed-bitwise
      -llvmlibc-restrict-system-libc-headers
      -modernize-macro-to-enum
      -readability-avoid-const-params-in-decls
      -readability-duplicate-include
      -readability-function-cognitive-complexity
      -readability-function-size
      -readability-identifier-length
      -readability-isolate-declaration
      -readability-magic-numbers
      -readability-non-const-parameter)
  string(REPLACE ";" "," CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS}")
  set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY}" -checks=${CLANG_TIDY_CHECKS}
  -system-headers -header-filter=${CMAKE_SOURCE_DIR})
else()
  message(WARNING "clang-tidy not found, static analysis will be skipped.")
endif()

find_package(Threads REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include
                    ${CMAKE_SOURCE_DIR}/include/utils)

add_subdirectory(external/SQLiteCPP SYSTEM)
add_library(components OBJECT src/uart.cpp src/utils/msg_parser.cpp)

add_executable(uart_handler src/main.cpp src/db_handler.cpp)
target_link_libraries(uart_handler PUBLIC components Threads::Threads SQLiteCpp
                                          sqlite3 pthread dl)
