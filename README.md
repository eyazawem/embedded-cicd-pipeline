# embedded-cicd-pipeline

[![CI](https://github.com/eyazawem/embedded-cicd-pipeline/actions/workflows/ci.yml/badge.svg)](https://github.com/eyazawem/embedded-cicd-pipeline/actions/workflows/ci.yml)

A C++17 signal-processing library delivered through a reproducible CI pipeline:
formatting check, static analysis, build, unit tests, coverage gate and a
minimal runtime container image. Every pipeline step is a script that runs
inside the same Docker build image locally, on GitHub Actions and on Jenkins.

## What the software does

The library provides two building blocks typical of sensor signal handling:

- `cicd::MovingAverage`: moving average over the last N samples (circular buffer).
- `cicd::SignalValidator`: plausibility check of a sensor signal. A sample is
  `Valid`, `OutOfRange` (outside min/max, or NaN) or `RateExceeded` (change
  from the last valid sample above the allowed maximum). A rejected sample
  never becomes the reference for the next check.

`signal_demo` is a small program that chains both: it validates a speed signal
and averages the accepted samples.

## Pipeline

| Stage | Tool | Script | Gate |
|---|---|---|---|
| Formatting | clang-format 18 | `scripts/format.sh --check` | any difference fails |
| Static analysis | Cppcheck 2.13 | `scripts/static_analysis.sh` | any finding fails |
| Build and unit tests | CMake 3.28, Ninja, GoogleTest 1.14 | `scripts/build_and_test.sh` | any failing test fails; warnings are errors |
| Coverage | gcov, lcov 2.0 | `scripts/coverage.sh` | line coverage below `MIN_LINE_COVERAGE` (default 90) fails |
| Runtime image | Docker multi-stage | `scripts/build_runtime_image.sh` | smoke test of the image fails |

### GitHub Actions

`.github/workflows/ci.yml` builds the build image from `Dockerfile`, then runs
the scripts above in order. The coverage report is published as the
`coverage-report` artifact.

### Jenkins

The `Jenkinsfile` is a declarative pipeline that runs the same scripts in the
same order: build reference image, check formatting, static analysis, build and
test, coverage, runtime image. It keeps `coverage/**` as a build artifact,
allows one build at a time, limits a build to 30 minutes and cleans the
workspace at the end.

Setup used for this project (Ubuntu 20.04 host, Jenkins installed natively
from the official LTS apt repository):

- Jenkins 2.580.1. This version refuses to start on Java 17 and requires Java
  21 or 25; Temurin 21 was installed from the Adoptium apt repository.
- The `jenkins` user belongs to the `docker` group so that the scripts can run
  containers. Membership in the `docker` group is equivalent to root access on
  the host: acceptable on a personal test machine, not on a shared server.
- The job is a Pipeline "from SCM" on this repository, branch `main`, script
  path `Jenkinsfile`. It polls the repository every 5 minutes (`H/5 * * * *`)
  because the host is not reachable from GitHub, so webhooks cannot be used.

## Run it locally

Requirements: Docker (tested with Docker 26.1.3 on Ubuntu 20.04) and Git.

```bash
git clone git@github.com:eyazawem/embedded-cicd-pipeline.git
cd embedded-cicd-pipeline

docker build -t embedded-cicd-builder:24.04 .   # reference build image

./scripts/format.sh --check        # formatting check (run without --check to fix)
./scripts/static_analysis.sh       # Cppcheck
./scripts/build_and_test.sh        # build, 21 unit tests, demo program
./scripts/coverage.sh              # coverage report in coverage/html/index.html
./scripts/build_runtime_image.sh   # runtime image + smoke tests
```

The threshold can be changed: `MIN_LINE_COVERAGE=95 ./scripts/coverage.sh`.

## Docker images

- `Dockerfile`: build image based on Ubuntu 24.04 with g++ 13.3, CMake 3.28.3,
  Ninja 1.11.1, Cppcheck 2.13.0, clang-format 18.1.3, lcov 2.0, GoogleTest 1.14.0.
- `Dockerfile.runtime`: multi-stage build. The program is compiled in the build
  image (Release), then copied into a plain `ubuntu:24.04` image that runs it
  as a non-root user (uid 10001). The runtime image has no compiler and no
  build tools (measured: 78.2 MB, against 760 MB for the build image).

## Repository layout

```text
.
├── .github/workflows/ci.yml   GitHub Actions pipeline
├── Jenkinsfile                Jenkins pipeline
├── CMakeLists.txt
├── Dockerfile                 reference build image
├── Dockerfile.runtime         multi-stage runtime image
├── include/cicd/              public headers
├── src/                       library and demo program
├── tests/                     GoogleTest unit tests
└── scripts/                   every pipeline step, runnable locally
```

## Design notes and limitations

- CI and local runs use the same scripts and the same image, so a result
  obtained on a developer machine is the result CI will produce.
- Compiler warnings (`-Wall -Wextra -Wpedantic`) are treated as errors.
- Coverage is line and function coverage. Branch coverage is not gated.
- With lcov 2.0 and GoogleTest, `geninfo` reports "mismatched end line"
  for multi-line test macros. The collection step runs with
  `--ignore-errors mismatch`; the affected files are under `tests/`, which is
  excluded from the report, as is `src/main.cpp`.
- Cppcheck analyses the production code in `src/`, not the tests.
- The runtime image is built and smoke-tested in CI but not pushed to a
  container registry.
