cmake_minimum_required(VERSION 3.10)
project(CxxLox)

# Set C++20 as the standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Catch2 library (if available)
find_package(Catch2 QUIET)

# Option to compile with symbols
option(WITH_SYMBOLS "Compile with debugging symbols" OFF)
if(WITH_SYMBOLS)
    message(STATUS "Compiling with debugging symbols")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g") # or -ggdb for gdb
endif()

# Option to enable AddressSanitizer
option(USE_ASAN "Enable AddressSanitizer" OFF)
if(USE_ASAN)
    message(STATUS "Enabling AddressSanitizer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")  # For GCC/Clang
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")  
endif()

#include 
include_directories(include) 

source_group("Header Files" FILES include/Interpreter/Scanner.h include/Interpreter/Tokens.h)

# Add subdirectories
add_subdirectory(src/Interpreter)
add_subdirectory(src/ast_tool)

# Add tests (if Catch2 is found)
if(CATCH_FOUND)
    enable_testing()
    add_subdirectory(tests)
endif()
