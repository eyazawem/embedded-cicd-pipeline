# Contributing

## Workflow

1. Create a branch from `main`. Use a prefix that matches the change:
   `feat/`, `fix/`, `docs/`, `chore/`, `ci/`.
2. Make small, focused commits that follow
   [Conventional Commits 1.0.0](https://www.conventionalcommits.org/en/v1.0.0/):
   `<type>(<scope>): <description>`. Types used in this repository:
   `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `build`, `ci`, `chore`.
3. Run the checks locally. They all run in the reference Docker image:

   ```bash
   docker build -t embedded-cicd-builder:24.04 .
   ./scripts/format.sh
   ./scripts/static_analysis.sh
   ./scripts/build_and_test.sh
   ./scripts/coverage.sh
   ```

4. Open a pull request against `main`. The CI workflow must pass before the
   change is merged.

## Standards

- Versioning: [Semantic Versioning 2.0.0](https://semver.org/spec/v2.0.0.html).
  A release is a tag `vMAJOR.MINOR.PATCH`; pushing it triggers the release
  workflow.
- Changelog: [Keep a Changelog 1.1.0](https://keepachangelog.com/en/1.1.0/).
  Add entries under "Unreleased" in `CHANGELOG.md`.
- Code: C++17, formatted with clang-format (`.clang-format`), compiler warnings
  treated as errors.
- Tests: every behavior change comes with unit tests. The coverage gate requires
  at least 90% of lines.
