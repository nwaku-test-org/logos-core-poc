set(CMAKE_AUTOMOC ON)

# Define the library sources
set(LOGOS_CORE_SOURCES
    logos_core.cpp
    logos_core.h
    ../interface.h
    ../plugin_registry.h
)

# Define the host application sources
set(HOST_SOURCES
    main.cpp
)

# Create the logos core library
add_library(logos_core SHARED ${LOGOS_CORE_SOURCES})

# Set the LOGOS_CORE_LIBRARY definition for the library
target_compile_definitions(logos_core PRIVATE LOGOS_CORE_LIBRARY)

# Link Qt libraries to the library
target_link_libraries(logos_core PRIVATE Qt${QT_VERSION_MAJOR}::Core)

# Include directories for the library
target_include_directories(logos_core PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${Qt${QT_VERSION_MAJOR}_INCLUDE_DIRS}
)

# Create the host application
add_executable(logoscore ${HOST_SOURCES})

# Link the host application with the logos core library
target_link_libraries(logoscore PRIVATE logos_core)

# Include directories for the host application
target_include_directories(logoscore PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${Qt${QT_VERSION_MAJOR}_INCLUDE_DIRS}
) 