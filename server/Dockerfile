FROM alpine:latest AS builder
RUN apk add build-base
RUN mkdir /p2p-server
WORKDIR /p2p-server
COPY ./server/include ./include
COPY ./server/src ./src
COPY ./server/test ./test
#COPY ./server/lib ./lib
COPY ./cpp-common-lib ./lib
COPY ./server/build.sh ./server/unittest.sh ./
RUN sh unittest.sh
RUN sh build.sh

FROM alpine:latest
COPY --from=builder /p2p-server/bin/p2p-server /p2p-server/bin/p2p-server
RUN apk add --no-cache libstdc++
ENTRYPOINT ["/p2p-server/bin/p2p-server"]
