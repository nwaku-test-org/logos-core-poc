#!/bin/bash

# build_app_plugins.sh
# Script to build plugins from plugins_app/ and place them in plugins_app/build/plugins/

set -e  # Exit on error

echo "Building app plugins..."

# Make sure the plugins output directory exists
mkdir -p plugins_app/build/plugins

# List of plugins to build
plugins=("counter" "colorchanger" "waku_ui")

# Build each plugin individually
for plugin_name in "${plugins[@]}"; do
    plugin_dir="plugins_app/$plugin_name"
    
    if [ ! -d "$plugin_dir" ]; then
        echo "Warning: Plugin directory $plugin_dir does not exist. Skipping."
        continue
    fi
    
    echo "Building plugin: $plugin_name"
    
    # Create build directory for this plugin
    mkdir -p "$plugin_dir/build"
    
    # Build the plugin
    (
        cd "$plugin_dir/build"
        cmake .. -DCMAKE_BUILD_TYPE=Release
        make -j$(sysctl -n hw.ncpu)
    )
done

# Clean up existing plugins if any
rm -f plugins_app/build/plugins/*.dylib plugins_app/build/plugins/*.so

# Copy the built plugins to the central plugins directory
echo "Copying plugins to plugins_app/build/plugins/..."

# Find all built plugin libraries and copy them
if [[ "$(uname)" == "Darwin" ]]; then
    # macOS: .dylib files
    for plugin_name in "${plugins[@]}"; do
        plugin_dir="plugins_app/$plugin_name"
        if [ -d "$plugin_dir/build" ]; then
            find "$plugin_dir/build" -name "*.dylib" -type f -exec cp {} plugins_app/build/plugins/ \;
        fi
    done
    
    # Set correct permissions if any files were copied
    find plugins_app/build/plugins -name "*.dylib" -type f -exec chmod 755 {} \;
else
    # Linux/other: .so files
    for plugin_name in "${plugins[@]}"; do
        plugin_dir="plugins_app/$plugin_name"
        if [ -d "$plugin_dir/build" ]; then
            find "$plugin_dir/build" -name "*.so" -type f -exec cp {} plugins_app/build/plugins/ \;
        fi
    done
    
    # Set correct permissions if any files were copied
    find plugins_app/build/plugins -name "*.so" -type f -exec chmod 755 {} \;
fi

echo "Looking for plugin libraries in plugins_app/build/plugins directory..."
find plugins_app/build/plugins -type f \( -name "*.dylib" -o -name "*.so" -o -name "*.dll" \) | while read plugin; do
    echo "Copying plugin: $plugin"
    cp "$plugin" "app/build/plugins/"
done

echo "App plugins built successfully!"
echo "Plugins are available in: plugins_app/build/plugins/" 