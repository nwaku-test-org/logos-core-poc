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
    interfaces/IComponent.h
    resources.qrc
)

qt_add_executable(LogosApp
    ${PROJECT_SOURCES}
)

target_link_libraries(LogosApp PRIVATE Qt6::Widgets)

# Create plugins directory in build folder and copy plugins
add_custom_command(TARGET LogosApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/plugins
    COMMAND ${CMAKE_COMMAND} -E echo "Creating plugins directory at ${CMAKE_CURRENT_BINARY_DIR}/plugins"
)

# Copy plugins from component-system to our app's plugins directory
add_custom_command(TARGET LogosApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Copying plugins from ${CMAKE_SOURCE_DIR}/../reference/component-system/build/app/plugins/ to ${CMAKE_CURRENT_BINARY_DIR}/plugins/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/../reference/component-system/build/app/plugins/counter.dylib
        ${CMAKE_SOURCE_DIR}/../reference/component-system/build/app/plugins/colorchanger.dylib
        ${CMAKE_CURRENT_BINARY_DIR}/plugins/
) 