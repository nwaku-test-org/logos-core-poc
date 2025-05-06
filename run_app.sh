#!/bin/bash

# Exit on error
set -e

echo "Building core..."
./run_core.sh build

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
mkdir -p bin/modules

# Check if the "all" argument was provided
if [ "$1" = "all" ]; then
    cd ../..
    echo "Building plugins first..."
    ./build_core_modules.sh
    ./build_app_plugins.sh
    cd app/build
fi

# Run the application
echo "Starting application..."
./LogosApp

echo "Application closed." 