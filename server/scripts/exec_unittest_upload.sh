#!/bin/bash

BASE_DIR=$(dirname $0)
cd "${BASE_DIR}/../.."

# Upload the server_ut.info lcov file to codecov.io
bash <(curl -s https://codecov.io/bash) -f lcov/server_ut.info
