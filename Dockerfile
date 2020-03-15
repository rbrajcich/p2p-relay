FROM ubuntu:18.04
RUN apt-get update
RUN apt-get install -y g++
RUN mkdir /p2p-server
WORKDIR /p2p-server
