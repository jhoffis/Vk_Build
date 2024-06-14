#!/bin/bash -i
shopt -s expand_aliases
cd debug
if ! [ -z "$1" ]; then
    echo "rebuild cmake"
    cmake -DCMAKE_BUILD_TYPE=Debug ..
fi
make
./Vulkan
