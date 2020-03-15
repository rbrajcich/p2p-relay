#!/bin/bash

cd /p2p-relay/server

# Delete build directories if they already exist
if [ -d bin ]; then rm -rf bin; fi

# Make build output directories
mkdir bin

# Create executable in bin/
g++ -I ./src ./src/*.cpp -o ./bin/p2p-server
