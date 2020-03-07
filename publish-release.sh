#!/bin/bash

echo $DOCKERHUB_TOKEN | docker login -u rbrajcich --password-stdin
docker push rbrajcich/p2p-relay-server
