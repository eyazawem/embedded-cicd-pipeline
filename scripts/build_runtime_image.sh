#!/usr/bin/env bash
# Build the runtime image (multi-stage) and run smoke tests on it.
# Requires the reference build image (see Dockerfile) to exist locally.
set -euo pipefail

BUILDER_IMAGE="embedded-cicd-builder:24.04"
RUNTIME_IMAGE="${RUNTIME_IMAGE:-embedded-cicd-demo:dev}"
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "${PROJECT_DIR}"

docker build -f Dockerfile.runtime \
    --build-arg BUILDER_IMAGE="${BUILDER_IMAGE}" \
    -t "${RUNTIME_IMAGE}" .

echo "--- smoke test: program output"
OUTPUT="$(docker run --rm "${RUNTIME_IMAGE}")"
echo "${OUTPUT}"
echo "${OUTPUT}" | grep -q "sample=300 status=OUT_OF_RANGE"
echo "${OUTPUT}" | grep -q "sample=95 status=RATE_EXCEEDED"

echo "--- smoke test: container runs as non-root"
test "$(docker run --rm --entrypoint id "${RUNTIME_IMAGE}" -u)" = "10001"

echo "--- image sizes"
docker image ls | grep -E "REPOSITORY|embedded-cicd"

echo "OK: runtime image smoke tests passed"
