file(GLOB GRAPHICS_SOURCES
    "*.cpp"
)

add_library(battery_lib STATIC ${BATTERY_SOURCES})

target_include_directories(battery_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)