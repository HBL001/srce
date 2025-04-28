file(GLOB GRAPHICS_SOURCES
    "*.cpp"
)

add_library(graphics_lib STATIC ${GRAPHICS_SOURCES})

target_include_directories(graphics_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)