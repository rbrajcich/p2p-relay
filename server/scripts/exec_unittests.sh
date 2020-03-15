#!/bin/bash

cd /p2p-relay/server

# Delete build directories if they already exist
if [ -d bin ]; then rm -rf bin; fi
if [ -d obj ]; then rm -rf obj; fi

# Make build output directories
mkdir bin
mkdir obj
mkdir obj/test

# Find possible library locations if they are present
if [ -d ./lib ]
then
LIB_DIR="${LIB_DIR} ./server/lib/**/*.a"
fi

if [ -d ../cpp-common-lib ]
then
LIB_DIR="${LIB_DIR} ./cpp-common-lib/**/*.a"
fi

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
cd ../../..

# Build executable with gcov
g++ -DP2P_RELAY_UNIT_TEST=1 -I ./server/test \
    -I ./server/include -I ./server/src -fprofile-arcs \
    -ftest-coverage ./server/obj/*.o \
    ./server/obj/test/*.o $LIB_DIR -o \
    ./server/bin/p2p-server-test -pthread

# Run the UTs
exec ./server/bin/p2p-server-test