#!/usr/bin/env bash
# Format all C++ sources with clang-format (default), or only check them (--check).
set -euo pipefail

IMAGE="embedded-cicd-builder:24.04"
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

case "${1:-fix}" in
    --check) CF_ARGS="--dry-run --Werror" ;;
    fix)     CF_ARGS="-i" ;;
    *)       echo "Usage: $0 [--check]" >&2; exit 2 ;;
esac

docker run --rm \
    --user "$(id -u):$(id -g)" \
    -v "${PROJECT_DIR}:/workspace" \
    -w /workspace \
    "${IMAGE}" \
    bash -c "find include src tests -type f \( -name '*.cpp' -o -name '*.hpp' \) -print0 \
        | xargs -0 clang-format ${CF_ARGS}"
