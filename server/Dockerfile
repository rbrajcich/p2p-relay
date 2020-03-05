FROM alpine:latest

RUN apk add build-base
RUN cd /
RUN mkdir p2p-server

COPY ./src /p2p-server/src
COPY ./test /p2p-server/test
COPY build.sh /p2p-server/build.sh

RUN sh /p2p-server/build.sh
ENTRYPOINT ["/p2p-server/bin/p2p-server"]
