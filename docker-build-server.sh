#!/bin/bash

if [ -d $1 ]
then
docker build -f server/Dockerfile .
else
docker build -f server/Dockerfile --tag $1 .
fi

