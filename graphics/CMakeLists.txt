# CMakeLists.txt for FT800 SDK in /graphics

# List source files
set(GRAPHICS_SRC
    ft800.cpp
    ft800If.cpp
    spiIf.cpp
    touch.cpp
)

# List public headers (for IDEs and install/export targets)
set(GRAPHICS_HEADERS
    ft800.h
    ft800If.h
    spiIf.h
    touch.h
    ft800Reg.h
    ft800Formats.h
    ft800_constants.h
    device_definitions.h
)

# Create static library target
add_library(graphics_lib STATIC ${GRAPHICS_SRC} ${GRAPHICS_HEADERS})

# Include current directory for headers
target_include_directories(graphics_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Use C++17
target_compile_features(graphics_lib PUBLIC cxx_std_17)
