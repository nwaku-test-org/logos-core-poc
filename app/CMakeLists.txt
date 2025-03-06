cmake_minimum_required(VERSION 3.16)
project(LogosApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)  # Enable Qt's Meta-Object Compiler
set(CMAKE_AUTORCC ON)  # Enable automatic compilation of resource files
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

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
    resources.qrc
)

qt_add_executable(LogosApp
    ${PROJECT_SOURCES}
)

target_link_libraries(LogosApp PRIVATE Qt6::Widgets) 