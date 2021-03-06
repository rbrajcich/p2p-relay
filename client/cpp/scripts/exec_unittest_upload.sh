#!/bin/bash

# Get to root of repository
BASE_DIR=$(dirname $0)
cd "${BASE_DIR}/../../.."

# Upload the client_ut.info lcov file to codecov.io
bash <(curl -s https://codecov.io/bash) -f lcov/client_ut.info
