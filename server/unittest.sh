#!/bin/bash

BASE_DIR=$(dirname $0)

cd $BASE_DIR

# Delete build directories if they already exist
if [ -d bin ]; then rm -rf bin; fi

# Make build output directories
mkdir bin

# Lib location could include 2 different paths if they exist
LIB_DIR=""

if [ -d ./lib ]
then
LIB_DIR="${LIB_DIR} ./lib/**/*.a"
fi

if [ -d ../cpp-common-lib ]
then
LIB_DIR="${LIB_DIR} ../cpp-common-lib/**/*.a"
fi

# Create executable in bin/ linking UT libs
g++ -DP2P_RELAY_UNIT_TEST=1 -I ./src -I ./include \
    ./src/*.cpp ./test/*.cpp $LIB_DIR -o \
    ./bin/p2p-server-test -pthread

# Run the UTs
./bin/p2p-server-test