#!/bin/bash

BASE_DIR=$(dirname $0)
cd "${BASE_DIR}/.."

# Delete build directories if they already exist
if [ -d bin ]; then rm -rf bin; fi
if [ -d obj ]; then rm -rf obj; fi

# Make build output directories
mkdir bin
mkdir obj
mkdir obj/test

cd obj

# Build obj files for src/ with gcov
g++ -c -DP2P_RELAY_UNIT_TEST=1 -I ../src -I ../include \
    -fprofile-arcs -ftest-coverage \
    ../src/*.cpp

cd test

# Build obj files for test/ with gcov
g++ -c -DP2P_RELAY_UNIT_TEST=1 -I ../../test \
    -I ../../include -I ../../src \
    ../../test/*.cpp

# Navigate to root of repository
cd ../../../..

# Build executable with gcov included
g++ -DP2P_RELAY_UNIT_TEST=1 -I ./client/cpp/src -I ./client/cpp/include \
    -I ./client/cpp/test -fprofile-arcs -ftest-coverage ./client/cpp/obj/*.o \
    ./client/cpp/obj/test/*.o ./cpp-common-lib/**/*.a \
    -o ./client/cpp/bin/p2p-client-test -pthread

# Execute unit tests
./client/cpp/bin/p2p-client-test