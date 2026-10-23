#!/usr/bin/env bash
# Runs INSIDE the build image: instrumented build, tests, lcov report, threshold gate.
set -euo pipefail

MIN_LINE_COVERAGE="${MIN_LINE_COVERAGE:-90}"
BUILD_DIR="build-cov"
OUT_DIR="coverage"

rm -rf "${BUILD_DIR}" "${OUT_DIR}"
mkdir -p "${OUT_DIR}"

cmake -S . -B "${BUILD_DIR}" -G Ninja -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON
cmake --build "${BUILD_DIR}"
ctest --test-dir "${BUILD_DIR}" --output-on-failure

lcov --capture --ignore-errors mismatch --directory "${BUILD_DIR}" --output-file "${OUT_DIR}/coverage.raw.info"
lcov --remove "${OUT_DIR}/coverage.raw.info" '/usr/*' '*/tests/*' '*/src/main.cpp' \
     --ignore-errors unused --output-file "${OUT_DIR}/coverage.info"
genhtml "${OUT_DIR}/coverage.info" --output-directory "${OUT_DIR}/html"

awk -F: -v min="${MIN_LINE_COVERAGE}" '
    /^LF:/  { lf  += $2 }
    /^LH:/  { lh  += $2 }
    /^FNF:/ { fnf += $2 }
    /^FNH:/ { fnh += $2 }
    END {
        if (lf == 0) { print "ERROR: no instrumented lines found"; exit 2 }
        pct = 100.0 * lh / lf
        printf "Line coverage: %.1f%% (%d/%d lines), threshold %s%%\n", pct, lh, lf, min
        if (fnf > 0) {
            printf "Function coverage: %.1f%% (%d/%d functions)\n", 100.0 * fnh / fnf, fnh, fnf
        }
        if (pct + 0 < min + 0) { print "FAIL: line coverage below threshold"; exit 1 }
        print "OK: line coverage meets threshold"
    }' "${OUT_DIR}/coverage.info"
