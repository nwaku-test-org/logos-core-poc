cmake_minimum_required(VERSION 3.16)
project(ColorChangerPlugin VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Try to find the component-interfaces package first
find_package(component-interfaces QUIET)

# If not found, use the app/interfaces folder
if(NOT component-interfaces_FOUND)
    # Include the app/interfaces directory
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../app/interfaces)
    
    # Create a component-interfaces library
    add_library(component-interfaces INTERFACE)
    target_include_directories(component-interfaces INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../app/interfaces)
endif()

set(SOURCES
    ColorChangerComponent.cpp
    ColorChangerWidget.cpp
)

add_library(colorchanger SHARED ${SOURCES})

# Add include directories before target_link_libraries
target_include_directories(colorchanger PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../core
)

target_link_libraries(colorchanger 
    PRIVATE 
        Qt6::Core 
        Qt6::Widgets
        component-interfaces
)

# For macOS, we need to set the bundle properties
if(APPLE)
    set_target_properties(colorchanger PROPERTIES
        BUNDLE FALSE
        FRAMEWORK FALSE
        PREFIX ""
        SUFFIX ".dylib"
    )
else()
    set_target_properties(colorchanger PROPERTIES
        PREFIX ""
        SUFFIX ".so"
    )
endif() 