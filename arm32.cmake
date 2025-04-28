# Define the system name and processor architecture
SET(CMAKE_SYSTEM_PROCESSOR cortex-a8)
set(CMAKE_SYSTEM_NAME Linux)
#set(CMAKE_SYSTEM_PROCESSOR arm)

# If SDK_ROOT is not defined, use a default path
if(NOT DEFINED SDK_ROOT)
    set(SDK_ROOT "/opt/poky/5.0.4")
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

