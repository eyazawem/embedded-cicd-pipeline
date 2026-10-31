# Changelog

All notable changes to this project are documented in this file.

The format follows [Keep a Changelog 1.1.0](https://keepachangelog.com/en/1.1.0/)
and the project adheres to [Semantic Versioning 2.0.0](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Dependabot updates for GitHub Actions.
- Code owners, security policy, contributing guide and pull request template.
- Editor and Git attribute configuration.

## [1.0.0] - 2026-10-30

### Added

- `MovingAverage` and `SignalValidator` C++17 library with 21 unit tests.
- `signal_demo` program chaining the validator and the moving average.
- Reference build image (Ubuntu 24.04) and multi-stage runtime image running as
  a non-root user.
- GitHub Actions workflow: formatting check, Cppcheck, build and tests,
  coverage gate (90% of lines) and runtime image smoke test.
- Jenkinsfile running the same stages with the same scripts.
- Release workflow publishing the binary, the coverage report and SHA-256
  checksums on version tags.

[Unreleased]: https://github.com/eyazawem/embedded-cicd-pipeline/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/eyazawem/embedded-cicd-pipeline/releases/tag/v1.0.0
