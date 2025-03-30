cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(mainCode src/main.cpp)

#add_subdirectory(InstrumentSoftware)

# Use the toolchain file for cross-compilation
if(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling Yocto SDK")
endif()

