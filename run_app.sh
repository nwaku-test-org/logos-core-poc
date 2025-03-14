#!/bin/bash

# Exit on error
set -e

echo "Building and running Logos Core POC application..."

# Create build directory if it doesn't exist
mkdir -p app/build

# Navigate to build directory
cd app/build

# Run CMake if CMakeCache.txt doesn't exist (first time build)
if [ ! -f "CMakeCache.txt" ]; then
    echo "Running CMake..."
    cmake ..
fi

# Build the application
echo "Building application..."
make

# Create the plugins directory if it doesn't exist
echo "Setting up plugins directory..."
mkdir -p plugins
mkdir -p bin/plugins

# Copy the plugins from plugins/build/plugins to app/build/bin/plugins
echo "Copying plugins to app/build/bin/plugins/..."
if [[ "$(uname)" == "Darwin" ]]; then
    # Check if source directory exists
    if [ -d "../../plugins/build/plugins" ]; then
        # Copy .dylib files (macOS)
        find ../../plugins/build/plugins -name "*.dylib" -type f -exec cp {} bin/plugins/ \;
        # Set correct permissions
        find bin/plugins -name "*.dylib" -type f -exec chmod 755 {} \;
        # If running on macOS, update library paths for plugins if needed
        if [ -x "$(command -v install_name_tool)" ]; then
            for plugin in bin/plugins/*.dylib; do
                if [ -f "$plugin" ]; then
                    echo "Fixing library paths for $plugin"
                    install_name_tool -id @executable_path/../plugins/$(basename $plugin) $plugin
                fi
            done
        fi
    else
        echo "Warning: plugins/build/plugins directory not found. No plugins were copied."
    fi
else
    # Check if source directory exists
    if [ -d "../../plugins/build/plugins" ]; then
        # Copy .so files (Linux/other)
        find ../../plugins/build/plugins -name "*.so" -type f -exec cp {} bin/plugins/ \;
        # Set correct permissions
        find bin/plugins -name "*.so" -type f -exec chmod 755 {} \;
    else
        echo "Warning: plugins/build/plugins directory not found. No plugins were copied."
    fi
fi

# Run the application
echo "Starting application..."
./LogosApp

echo "Application closed." 