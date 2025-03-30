## Using the Toolchain ##

To utilize your wrlinux_toolchain.cmake file for cross-compiling your C++ code in the ~/wrlinux-source directory using CMake, follow these steps:

### 1. Ensure the Toolchain File is Correct:

Your wrlinux_toolchain.cmake file should accurately define the cross-compilation environment. Here's an example configuration:

```
# Specify the target system name and architecture
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Define the cross compiler executables
set(CMAKE_C_COMPILER arm-wrs-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER arm-wrs-linux-gnueabi-g++)

# Specify the sysroot path
set(CMAKE_SYSROOT /opt/wrlinux-toolchain/sysroots/armv7at2hf-neon-wrs-linux-gnueabi)

# Adjust the search paths for the compiler
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})

# Configure the find commands to search only in the specified paths
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

```

### 2. Invoke CMake with the Toolchain File: 

Navigate to your build directory and run CMake, specifying your toolchain file:

```
cd ~/wrlinux-source
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../wrlinux_toolchain.cmake ..

```
This command directs CMake to use the specified toolchain file for configuring the project.

### 3. Build the Project:

After configuration, compile your project using:

```
make -j4 #(if you have a good PC this can be 16)
```

This process will generate binaries tailored for the target architecture defined in your toolchain file.

### Additional Considerations:

Environment Variables: Ensure that the cross-compilation tools (e.g., arm-wrs-linux-gnueabi-gcc) are accessible in your system's PATH. If they're located in a non-standard directory, you might need to adjust the CMAKE_PREFIX_PATH or update your PATH environment variable accordingly.

CMake Version: Verify that you're using a compatible version of CMake that supports the features required for your project.

## Project Directory Structure

Organizing your project directory systematically enhances maintainability and scalability. A recommended structure is:

```
project-root/
├── CMakeLists.txt
├── cmake/
│   └── wrlinux_toolchain.cmake
├── src/
│   ├── main.cpp
│   └── CMakeLists.txt
├── include/
│   └── project_name/
│       └── header.hpp
├── build/
└── tests/
    ├── test_main.cpp
    └── CMakeLists.txt
```

CMakeLists.txt: Top-level CMake configuration file.
cmake/: Directory for CMake-related scripts and modules.
src/: Source files.
include/: Header files, organized under a directory named after your project.
build/: Directory where the build output will be generated.
tests/: Test source files and their CMake configurations.

## Top-Level CMakeLists.txt

This file orchestrates the build process. Here's an example:

```
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(CoreScanSoftware VERSION 0.1 LANGUAGES CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the toolchain file for cross-compilation
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/wrlinux_toolchain.cmake)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)

# Enable testing
enable_testing()
```

4. src/CMakeLists.txt

Configure the build for your source files:

```
# src/CMakeLists.txt

# Add executable
add_executable(${PROJECT_NAME} main.cpp)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Link libraries (if any)
# target_link_libraries(${PROJECT_NAME} PRIVATE some_library)
```

5. Building the Project

To build your project using the specified toolchain:

```
# Navigate to the build directory
cd /home/user/wrlinux-source/build

# Run CMake with the toolchain file
cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/wrlinux_toolchain.cmake

# Build the project
make
```











