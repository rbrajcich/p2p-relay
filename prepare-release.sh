#!/bin/bash

docker tag server-container:latest rbrajcich/p2p-relay-server:$1
docker tag server-container:latest rbrajcich/p2p-relay-server:latest
