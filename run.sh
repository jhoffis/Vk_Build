#!/bin/bash
shopt -s expand_aliases

# Change to the debug directory, exit if it fails
cd debug || { echo "Failed to change directory to 'debug'"; exit 1; }

# Check if a parameter is passed
if ! [ -z "$1" ]; then
    echo "rebuild cmake"
    cmake -DCMAKE_BUILD_TYPE=Debug ..
fi

# Build the project
make

# Run the appropriate command based on the parameter
if [ "$1" == "test" ]; then
    ./Vulkan test
else
    ./Vulkan
fi
