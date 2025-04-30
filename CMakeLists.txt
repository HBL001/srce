cmake_minimum_required(VERSION 3.10)
project(biobeagle)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Add subdirectories
add_subdirectory(graphics)
add_subdirectory(src)

# Cross-compiling notice
if(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling Yocto SDK")
endif()
