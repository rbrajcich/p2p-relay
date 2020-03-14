#!/bin/bash

BASE_DIR=$(dirname $0)

cd $BASE_DIR

# Delete build directories if they already exist
if [ -d obj ]; then rm -rf obj; fi
if [ -d release ]; then rm -rf release; fi

# Make build output directories
mkdir obj
mkdir release

# Navigate to obj dir
cd obj

exit 1

# Build object files and output in obj/ directory
g++ -c -I ../src ../src/*.cpp 

# Return to base dir
cd ..

# Create static lib in release/lib dir
mkdir release/lib 
ar -cvq release/lib/p2p-relay.a obj/*.o

# Clone client public API headers to release/include
mkdir release/include
cp -R include/p2p-relay release/include
