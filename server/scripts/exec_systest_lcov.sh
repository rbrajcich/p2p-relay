#!/bin/bash

BASE_DIR=$(dirname $0)
cd "${BASE_DIR}/../.."

if [ ! -d lcov  ]; then mkdir -m 777 lcov; fi
if [ ! -d lcov/server-systest-html  ]; then mkdir -m 777 lcov/server-systest-html; fi

# Compile the lcov file and generate html
lcov --capture --directory server --output-file lcov/server_systest.info
lcov --extract lcov/server_systest.info */server/src/* -o lcov/server_systest.info
genhtml lcov/server_systest.info --output-directory lcov/server-systest-html