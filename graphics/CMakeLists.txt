# CMakeLists.txt for FT800 SDK in /graphics

# List source files
set(GRAPHICS_SRC
    graph_ft800.cpp
    graph_touch.cpp
)
    
set(GRAPHICS_HEADERS
    graph_ft800.h
    graph_ft800_ioctl.h
    graph_ft800Reg.h
    graph_ft800Formats.h
    graph_ft800_constants.h
    graph_touch.h
    )

# Create static library target
add_library(graphics_lib STATIC ${GRAPHICS_SRC} ${GRAPHICS_HEADERS})

# Include current directory for headers
target_include_directories(graphics_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Use C++17
target_compile_features(graphics_lib PUBLIC cxx_std_17)
