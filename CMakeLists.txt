cmake_minimum_required(VERSION 3.16)
project(LLVMIRConverter LANGUAGES C CXX)
set(LLVM_USE_STATIC_LIBS OFF)

set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
link_directories(${LLVM_LIBRARY_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "")

if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()

add_library(llvm_graph_analysis SHARED 
            src/plugin.cpp)

target_link_libraries(llvm_graph_analysis LLVM)