#!/bin/bash

BASE_DIR=$(dirname $0)
cd "${BASE_DIR}/../.."

if [ ! -d lcov  ]; then mkdir -m 777 lcov; fi
if [ ! -d lcov/server-ut-html  ]; then mkdir -m 777 lcov/server-ut-html; fi

# Compile the lcov file and generate html
lcov --capture --directory server --output-file lcov/server_ut.info
lcov --extract lcov/server_ut.info */server/src/* -o lcov/server_ut.info
genhtml lcov/server_ut.info --output-directory lcov/server-ut-html