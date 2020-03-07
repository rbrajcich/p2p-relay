#!/bin/bash

BASE_DIR=$(dirname $0)

cd $BASE_DIR

# Delete build directories if they already exist
if [ -d bin ]; then rm -rf bin; fi

# Make build output directories
mkdir bin

# Build object files and output in obj/ directory
g++ -DP2P_RELAY_UNIT_TEST=1 -I ./src -I ./include \
    ./src/*.cpp ./test/*.cpp ../../cpp-common-lib/**/*.a \
    -o ./bin/p2p-client-test -pthread

# Execute unit tests
./bin/p2p-client-test