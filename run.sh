#!/bin/bash

# Check if build directory exists, if not create it
if [ ! -d "build" ]; then
  mkdir build
fi

# Change to the build directory
cd build

# Run cmake
cmake ..

# Run make
make

# Run the program
./car-rental