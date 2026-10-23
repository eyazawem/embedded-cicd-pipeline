#!/usr/bin/env bash
# Build and test the project inside the reference build image.
set -euo pipefail

IMAGE="embedded-cicd-builder:24.04"
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

docker run --rm \
    --user "$(id -u):$(id -g)" \
    -v "${PROJECT_DIR}:/workspace" \
    -w /workspace \
    "${IMAGE}" \
    bash -c 'cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug \
        && cmake --build build \
        && ctest --test-dir build --output-on-failure \
        && ./build/signal_demo'
