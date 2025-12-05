#!/bin/bash

# Build script for SearchEngine (macOS/Linux)
# Usage: ./build.sh

set -e

echo "Creating build directory..."
mkdir -p build
cd build

echo "Configuring CMake..."
cmake ..

echo "Building project..."
cmake --build .

echo ""
echo "Build complete! Executable is in: build/bin/SearchEngine"
echo "Run with: ./build/bin/SearchEngine"
echo ""
echo "The server will start on http://localhost:8080"
echo "Open your browser and navigate to that address to use the search engine."

