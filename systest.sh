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

./client/cpp/build systests

elif [ $1 == "generate-cov" ]; then

./client/cpp/build systest-cov

elif [ $1 == "upload-cov" ]; then

./client/cpp/build upload-systest-cov

else
echo "Error - Unrecognized option: ${1}"
echo "Usage: ${USAGE}"
fi
