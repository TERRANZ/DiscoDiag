FROM debian:bookworm-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        ninja-build \
        pkg-config \
        qt6-base-dev \
        qt6-base-dev-tools \
        qt6-connectivity-dev \
        qt6-tools-dev \
        qt6-tools-dev-tools \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
