#!/bin/bash

# Exit on error
set -e

echo "Building Logos Core plugins..."

# Get the script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

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

# Fix library paths for platform-specific dynamic libraries
if [ "$(uname)" = "Darwin" ]; then
    echo "Fixing library paths for waku_plugin.dylib on macOS..."
    # Use otool to get the current library path
    CURRENT_PATH=$(otool -L "plugins/waku_plugin.dylib" | grep libwaku.so | awk '{print $1}')
    
    # Update the paths using relative references
    install_name_tool -change "${CURRENT_PATH}" "@rpath/libwaku.so" "plugins/waku_plugin.dylib"
    install_name_tool -id "@rpath/libwaku.so" "plugins/libwaku.so"
elif [ "$(uname)" = "Linux" ]; then
    echo "Fixing library paths for waku_plugin.so on Linux..."
    # No need for install_name_tool on Linux
    # Set rpath directly for Linux shared objects
    if [ -f "plugins/waku_plugin.so" ]; then
        patchelf --set-rpath '$ORIGIN' "plugins/waku_plugin.so"
    fi
fi

# Go back to the root directory
cd ../../

# Copy plugin libraries to build plugins directory
echo "Copying plugin libraries to build plugins directory..."

# Create plugins directory in core build if it doesn't exist
mkdir -p core/build/plugins
mkdir -p app/build/bin/plugins

# Find all plugin libraries and copy them to the build plugins directory
echo "Looking for plugin libraries in plugins/build/plugins directory..."
find plugins/build/plugins -type f \( -name "*.dylib" -o -name "*.so" -o -name "*.dll" \) | while read plugin; do
    echo "Copying plugin: $plugin"
    cp "$plugin" "core/build/plugins/"
    cp "$plugin" "app/build/bin/plugins/"
done

echo "Plugins built successfully." 