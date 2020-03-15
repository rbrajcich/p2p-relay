#!/bin/bash

# Ensure we are working in repository root
BASE_DIR=$(dirname $0)
cd $BASE_DIR

USAGE="./systest.sh <exec|generate-cov|upload-cov>"

if [ $# != 1 ]; then
echo "Error - Incorrect number of arguments."
echo "Usage: ${USAGE}"
exit 1
fi

if [ $1 == "exec" ]; then

# Get the server container running
./server/build start-systest > /dev/null

# Run the systests from the client and stop the server once done
./client/cpp/build systests
./server/build stop-systest > /dev/null

elif [ $1 == "generate-cov" ]; then

./client/cpp/build systest-cov
./server/build systest-cov

elif [ $1 == "upload-cov" ]; then

echo "./client/cpp/build upload-systest-cov"
./client/cpp/build upload-systest-cov

echo "./server/build upload-systest-cov"
./server/build upload-systest-cov

else
echo "Error - Unrecognized option: ${1}"
echo "Usage: ${USAGE}"
fi
