#!/bin/bash

# prepare docker images for publishing
docker tag p2p-server-container:latest rbrajcich/p2p-relay-server:$1
docker tag p2p-server-container:latest rbrajcich/p2p-relay-server:latest

# prepare publishable archives for client lib assets
cd client-library-ubuntu
tar -czvf p2p-relay-client-ubuntu-v$1.tar.gz *