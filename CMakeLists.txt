cmake_minium_required(VERSION 3.16)
project(opt-visualizer LANGUAGES CXX)

find_package(LLVM REQUIRED CONFIG)

set(CMAKE_CXX_STANDARD 17)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_library(opt-passes MODULE
    src/passes/cse.cpp
)

target_link_libraries(opt-passes PRIVATE LLVM)

add_executable(opt-visualizer
    src/main.cpp
)

target_link_libraries(opt-visualizer PRIVATE LLVM)