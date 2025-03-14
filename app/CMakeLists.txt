cmake_minimum_required(VERSION 3.16)
project(LogosApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)  # Enable Qt's Meta-Object Compiler
set(CMAKE_AUTORCC ON)  # Enable automatic compilation of resource files
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Add interfaces directory to include path
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Add logos_core include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/host
    ${CMAKE_CURRENT_SOURCE_DIR}/../core
)

# Set logos_core library path
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../core/build/lib)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.h
    mainwindow.cpp
    sidebarbutton.h
    sidebarbutton.cpp
    mdichild.h
    mdichild.cpp
    mdiview.h
    mdiview.cpp
    modulesview.h
    modulesview.cpp
    dashboardview.h
    dashboardview.cpp
    coremoduleview.h
    coremoduleview.cpp
    interfaces/IComponent.h
    resources.qrc
)

qt_add_executable(LogosApp
    ${PROJECT_SOURCES}
)

# Link with logos_core library
target_link_libraries(LogosApp PRIVATE 
    Qt6::Widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/../core/build/lib/liblogos_core.dylib
)

# Create bin/plugins directory in build folder
add_custom_command(TARGET LogosApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/bin/plugins
    COMMAND ${CMAKE_COMMAND} -E echo "Creating plugins directory at ${CMAKE_CURRENT_BINARY_DIR}/bin/plugins"
)

# Build the plugins first
add_custom_command(TARGET LogosApp PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Building plugins..."
    COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/../plugins && mkdir -p build && cd build && cmake .. && make
)