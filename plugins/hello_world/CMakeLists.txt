set(CMAKE_AUTOMOC ON)

# Plugin sources
set(PLUGIN_SOURCES
    hello_world_plugin.cpp
    hello_world_plugin.h
    ${CMAKE_SOURCE_DIR}/../core/interface.h
)

# Create the plugin library
add_library(hello_world_plugin SHARED ${PLUGIN_SOURCES})

# Link Qt libraries
target_link_libraries(hello_world_plugin PRIVATE Qt${QT_VERSION_MAJOR}::Core)

# Include directories
target_include_directories(hello_world_plugin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../core
)

# Set properties for the plugin
set_target_properties(hello_world_plugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins"  # For Windows .dll
) 