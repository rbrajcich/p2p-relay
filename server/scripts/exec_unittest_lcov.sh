#!/bin/bash

cd /p2p-relay

if [ ! -d lcov  ]; then mkdir lcov; fi
if [ ! -d lcov/server-ut-html  ]; then mkdir lcov/server-ut-html; fi

chmod 777 lcov

# Compile the lcov file and generate html
lcov --capture --directory server --output-file lcov/server_ut.info
lcov --extract lcov/server_ut.info */server/src/* -o lcov/server_ut.info
genhtml lcov/server_ut.info --output-directory lcov/server-ut-html