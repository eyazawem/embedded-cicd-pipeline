#!/usr/bin/env bash
# Coverage report and threshold gate, executed in the reference build image.
# Usage: MIN_LINE_COVERAGE=90 ./scripts/coverage.sh
set -euo pipefail

IMAGE="embedded-cicd-builder:24.04"
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

docker run --rm \
    --user "$(id -u):$(id -g)" \
    -e HOME=/tmp \
    -e MIN_LINE_COVERAGE="${MIN_LINE_COVERAGE:-90}" \
    -v "${PROJECT_DIR}:/workspace" \
    -w /workspace \
    "${IMAGE}" \
    bash scripts/coverage_container.sh
