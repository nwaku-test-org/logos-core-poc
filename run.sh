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

# Run the application
echo "Starting application..."
./LogosApp

echo "Application closed." 