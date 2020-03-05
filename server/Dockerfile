FROM alpine:latest AS builder
RUN apk add build-base
RUN mkdir /p2p-server
WORKDIR /p2p-server
COPY ./src ./src
COPY ./test ./test
COPY build.sh ./build.sh
RUN sh build.sh

FROM alpine:latest
COPY --from=builder /p2p-server/bin/p2p-server /p2p-server/bin/p2p-server
RUN apk add --no-cache libstdc++
ENTRYPOINT ["/p2p-server/bin/p2p-server"]
