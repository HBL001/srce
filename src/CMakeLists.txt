# CMakeLists.txt for /src directory

# Add executable for the main application
add_executable(mainCode
    main.cpp
)

# Include headers from graphics
target_include_directories(mainCode PRIVATE
    ${CMAKE_SOURCE_DIR}/graphics
)

# Link against the FT800 SDK static library
target_link_libraries(mainCode PRIVATE
    graphics_lib
)

# Ensure C++17 is used
target_compile_features(mainCode PRIVATE cxx_std_17)
