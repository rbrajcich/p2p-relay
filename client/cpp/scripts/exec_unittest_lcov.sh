#!/bin/bash

# Start in p2p-server repository root
BASE_DIR=$(dirname $0)
cd "${BASE_DIR}/../../.."

if [ ! -d lcov  ]; then mkdir lcov; fi
if [ ! -d lcov/client-ut-html  ]; then mkdir lcov/client-ut-html; fi

# Compile the lcov file and generate html
lcov --capture --directory client/cpp --output-file lcov/client_ut.info
lcov --extract lcov/client_ut.info */client/cpp/src/* -o lcov/client_ut.info
genhtml lcov/client_ut.info --output-directory lcov/client-ut-html