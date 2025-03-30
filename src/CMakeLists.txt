cmake_minimum_required(VERSION 3.10)

# Specify the toolchain file for cross-compilation
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm32.cmake)

project(MyEmbeddedApp CXX)

# Add your executable (adjust source file names as needed)
add_executable(my_main src/main.cpp)

