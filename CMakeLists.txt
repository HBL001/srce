cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Add subdirectories
add_subdirectory(graphics)


# Main executable
add_executable(mainCode
    src/main.cpp
)

# Link libraries
target_link_libraries(mainCode PRIVATE
    graphics_lib
)

if(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling Yocto SDK")
endif()