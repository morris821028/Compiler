cmake_minimum_required(VERSION 3.2)
find_package(LLVM 3.7 REQUIRED CONFIG)
set(CMAKE_CXX_STANDARD 11)

add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "-fno-rtti")

add_subdirectory(MyDCE)
add_subdirectory(MyCSE)
