#!/usr/bin/env bash
# Static analysis of the production code with Cppcheck. Any finding fails the script.
set -euo pipefail

IMAGE="embedded-cicd-builder:24.04"
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

docker run --rm \
    --user "$(id -u):$(id -g)" \
    -v "${PROJECT_DIR}:/workspace" \
    -w /workspace \
    "${IMAGE}" \
    cppcheck --std=c++17 \
        --enable=warning,style,performance,portability \
        --error-exitcode=1 \
        --inline-suppr \
        -I include \
        src
