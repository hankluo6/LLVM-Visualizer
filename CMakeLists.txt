cmake_minimum_required(VERSION 3.16)
project(LLVMIRConverter LANGUAGES C CXX)
set(LLVM_USE_STATIC_LIBS OFF)

set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

# Include directories specified by LLVM in the project's include path
include_directories(${LLVM_INCLUDE_DIRS})
# Include definitions specified by LLVM in the project's options
add_definitions(${LLVM_DEFINITIONS})
link_directories(${LLVM_LIBRARY_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)

# Use the same C++ standard as LLVM does
set(CMAKE_CXX_STANDARD 17 CACHE STRING "")

# LLVM is normally built without RTTI. Be consistent with that.
if(NOT LLVM_ENABLE_RTTI)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
endif()
# Define the executable
# add_executable(llvm_ir_converter src/main.cpp)

add_library(llvm_graph_analysis SHARED 
            src/plugin.cpp)

target_link_libraries(llvm_graph_analysis
  PRIVATE
  LLVMCore
  LLVMPasses
  LLVMIRReader
  LLVMSupport
  LLVMAnalysis
  LLVMBinaryFormat
  LLVMBitWriter
  LLVMTransformUtils
  LLVMScalarOpts
)