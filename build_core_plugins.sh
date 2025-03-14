#!/bin/bash

# Exit on error
set -e

echo "Building Logos Core plugins..."

# Navigate to plugins directory
cd plugins

# Remove build to ensure clean build
if [ -d "build" ]; then
    echo "Removing old plugin build directory..."
    rm -rf build
fi

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Run CMake for plugins and point to the core library
echo "Running CMake for plugins..."
cmake .. -DCMAKE_PREFIX_PATH=../../core/build

# Build the plugins
echo "Building plugins..."
cmake --build .

# Go back to the root directory
cd ../../

# Copy plugin libraries to build plugins directory
echo "Copying plugin libraries to build plugins directory..."

# Create plugins directory in core build if it doesn't exist
mkdir -p core/build/plugins

# Find all plugin libraries and copy them to the build plugins directory
echo "Looking for plugin libraries in plugins/build directory..."
find plugins/build -type f \( -name "*.dylib" -o -name "*.so" -o -name "*.dll" \) | while read plugin; do
    echo "Copying plugin: $plugin"
    cp "$plugin" "core/build/plugins/"
done

echo "Plugins built successfully." 