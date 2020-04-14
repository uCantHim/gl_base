#! /usr/bin/env bash

if [[ ! -d build ]]; then mkdir build; fi
cd build || exit 1
rm -r ./*

if [[ ! -d gcc ]]; then mkdir gcc; fi
if [[ ! -d clang ]]; then mkdir clang; fi

# Compile with gcc
echo "--- Compiling with gcc..."
cd gcc || exit 1
export CXX=/usr/bin/gcc
if [[ ! $( cmake ../.. ) ]]; then
    echo "Cmake configuration failed. Exiting."
    exit 1
fi

cmake --build . --parallel 8 1>&2

# Compile with gcc
echo "--- Compiling with clang..."
cd ../clang || exit 1
export CXX=/usr/bin/clang++
if [[ ! $( cmake ../.. ) ]]; then
    echo "Cmake configuration failed. Exiting."
    exit 1
fi

cmake --build . --parallel 8 1>&2
