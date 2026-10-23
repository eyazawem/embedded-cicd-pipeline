FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        ninja-build \
        git \
        ca-certificates \
        libgtest-dev \
        cppcheck \
        clang-format \
        lcov \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
