## Using the Toolchain ##

To utilize your AM335x toolchain.cmake file for cross-compiling your C++ code in the ~/srce directory using CMake, follow these steps:

### 1. Ensure the Toolchain File is Correct:

Your arm32.cmake file should accurately define the cross-compilation environment. Here's an example configuration:

```
# Define the system name and processor architecture
SET(CMAKE_SYSTEM_PROCESSOR cortex-a8)
set(CMAKE_SYSTEM_NAME Linux)
#set(CMAKE_SYSTEM_PROCESSOR arm)

# If SDK_ROOT is not defined, use a default path
if(NOT DEFINED SDK_ROOT)
    set(SDK_ROOT "/opt/poky/5.1.4")
endif()

# Set target sysroot
set(SYSROOT "${SDK_ROOT}/sysroots/cortexa8hf-neon-poky-linux-gnueabi")

# Set compilers using full SDK paths
set(CMAKE_C_COMPILER   "${SDK_ROOT}/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-gcc")
set(CMAKE_CXX_COMPILER "${SDK_ROOT}/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-g++")

# Set the sysroot explicitly
set(CMAKE_SYSROOT "${SYSROOT}")

# Tell CMake to search headers/libraries only in the sysroot
set(CMAKE_FIND_ROOT_PATH "${SYSROOT}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Optional: Set flags explicitly if CMake has trouble detecting them
set(CMAKE_C_FLAGS   "--sysroot=${SYSROOT} -O2 -march=armv7-a -mfpu=neon -mfloat-abi=hard")
set(CMAKE_CXX_FLAGS "--sysroot=${SYSROOT} -O2 -march=armv7-a -mfpu=neon -mfloat-abi=hard")

# intellisense 
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

```

### 2. Invoke CMake with the Toolchain File: 

Navigate to your build directory and run CMake, specifying your toolchain file:

```
cd ~/wrlinux-source
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../arm32.cmake ..

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
│       └── header.h
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
project(MyProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(mainCode src/main.cpp)

#add_subdirectory(InstrumentSoftware)

# Use the toolchain file for cross-compilation
if(CMAKE_CROSSCOMPILING)
    message(STATUS "Cross-compiling Yocto SDK")
endif()
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
cmake_minimum_required(VERSION 3.10)

# Specify the toolchain file for cross-compilation
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm32.cmake)

project(MyEmbeddedApp CXX)

# Add your executable (adjust source file names as needed)
add_executable(my_main src/main.cpp)

```











