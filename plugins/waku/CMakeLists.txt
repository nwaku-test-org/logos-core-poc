cmake_minimum_required(VERSION 3.10)
project(waku)

# Enable C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt packages
find_package(Qt6 COMPONENTS Core REQUIRED)
if (NOT Qt6_FOUND)
    find_package(Qt5 5.15 COMPONENTS Core REQUIRED)
endif()

# Set automoc on
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Add the library
add_library(waku SHARED
    waku.cpp
    waku.h
    waku_interface.h
)

# Find the libwaku library
find_library(LIBWAKU_PATH
    NAMES libwaku.so
    PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib
    NO_DEFAULT_PATH
)

if(NOT LIBWAKU_PATH)
    message(FATAL_ERROR "libwaku.so not found")
endif()

# Link against libwaku and Qt
target_link_libraries(waku PRIVATE 
    ${LIBWAKU_PATH}
    Qt::Core
)

# Include directories
target_include_directories(waku PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/../..
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Set the output directory to match other plugins
set_target_properties(waku PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins") 