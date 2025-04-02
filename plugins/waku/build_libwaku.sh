#!/bin/bash

# Set base directory
SCRIPT_DIR="$(dirname "$0")"
NWAKU_DIR="$SCRIPT_DIR/vendor/nwaku"

# Check if vendor/nwaku directory exists
if [ ! -d "$NWAKU_DIR" ]; then
    echo "Error: vendor/nwaku directory not found. Run git submodule update --init --recursive"
    exit 1
fi

# Navigate to vendor/nwaku directory
cd "$NWAKU_DIR" || exit 1

# Build libwaku
echo "Running make libwaku..."
make libwaku

# Create lib directory if it doesn't exist
mkdir -p "$SCRIPT_DIR/lib"

# Copy libwaku.so to the lib directory
cp build/libwaku.so "$SCRIPT_DIR/lib/libwaku.so"

echo "Successfully built and copied libwaku.so"
