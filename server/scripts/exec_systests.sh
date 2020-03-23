#!/bin/bash

# Needed for !() glob syntax
shopt -s extglob

BASE_DIR=$(dirname $0)
cd "${BASE_DIR}/.."

# Delete build directories if they already exist
if [ -d bin ]; then rm -rf bin; fi
if [ -d obj ]; then rm -rf obj; fi

# Make build output directories
mkdir bin
mkdir -m 777 obj

cd obj

# Build obj files for src/ with gcov
g++ -c -DP2P_RELAY_SYS_TEST=1 -I ../src \
    -fprofile-arcs -ftest-coverage \
    ../src/*.cpp

# Navigate to root of repository
cd ../..

# Make directory for systest logs if not there
if [ ! -d logs  ]; then mkdir logs; fi

# Build executable with gcov included
g++ -DP2P_RELAY_SYS_TEST=1 -I ./server/src -I -fprofile-arcs -ftest-coverage \
    -lgcov --coverage ./server/obj/*.o ./cpp-common-lib/**/!(libgtest_main).a \
    -o ./server/bin/p2p-server-test -pthread

# Execute sys tests
exec ./server/bin/p2p-server-test