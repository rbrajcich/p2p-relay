#!/bin/bash

# Start in p2p-server repository root
BASE_DIR=$(dirname $0)
cd "${BASE_DIR}/../../.."

if [ ! -d lcov  ]; then mkdir lcov; fi
if [ ! -d lcov/client-systest-html  ]; then mkdir lcov/client-systest-html; fi

# Compile the lcov file and generate html
lcov --capture --directory client/cpp --output-file lcov/client_systest.info
lcov --extract lcov/client_systest.info */client/cpp/src/* -o lcov/client_systest.info
genhtml lcov/client_systest.info --output-directory lcov/client-systest-html