#!/bin/bash

# Exit on error
set -e

echo "Building and running Logos Core application..."

# First, build the core
echo "Building core application..."
cd core

# remove build to ensure clean build
if [ -d "build" ]; then
    echo "Removing old core build directory..."
    rm -rf build
fi

# Create build directory if it doesn't exist
mkdir -p build

# Navigate to build directory
cd build

# Run CMake
echo "Running CMake..."
cmake ..

# Build the application and core library
echo "Building core library and application..."
cmake --build .

# Create plugins directory in build
echo "Creating plugins directory..."
mkdir -p plugins

# Go back to the root directory
cd ../../

# Note: Plugin building has been moved to build_core_plugins.sh
# Make sure to run build_core_plugins.sh before this script if you need updated plugins

# Run the application
echo "Starting logoscore application..."
cd core/build/bin
./logoscore

echo "Application closed." 