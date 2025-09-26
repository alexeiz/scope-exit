# Repository Guidelines

## Project Structure & Module Organization
The library is header-only; ship everything from `include/`. Primary API lives in `include/scope_exit/scope_exit.hpp`, while version metadata is generated from `include/scope_exit/detail/version.hpp.in`. The library provides three scope guard types: `scope(exit)` for unconditional cleanup, `scope(success)` for success-only cleanup, and `scope(failure)` for exception-only cleanup. Tests reside in `test/` and use Catch2; adjust scenarios in `test/*.t.cpp` and register new binaries through `test/CMakeLists.txt`. Support tooling and presets sit in `cmake/`, and CMake generates build artifacts in `build/` (safe to purge). `src/` stays empty to emphasize the header-only design.

## Build, Test, and Development Commands
Initialize a debug workspace with `cmake --preset=default`, then compile via `cmake --build --preset=default`. Run the suite with `ctest --preset=default` or execute the binary directly using `./build/default/test/test_scope_exit`. For a turn-key cycle, `cmake --workflow --preset=default` orchestrates configure, build, and test. Produce release outputs with `cmake --preset=release && cmake --build --preset=release`, and install headers plus the package config using `cmake --install build/default` or `cmake --install build/release`. Alternative Clang-based builds are available via `--preset=default-clang` and `--preset=release-clang`. Single test execution: `./build/default/test/test_scope_exit` or `ctest --preset=default -R test_scope_exit`.

## Coding Style & Naming Conventions
Target C++17, four-space indent, and keep line lengths around 100 characters. Headers use lower_snake_case filenames; macros are UPPER_SNAKE_CASE; namespaces carry the version suffix (e.g., `scope_exit_v1`). Mirror existing naming such as `scope_guard` and avoid introducing dynamic allocation or external dependencies. Make sure all source files end with a new line.

## Testing Guidelines
Catch2 v3 (fetched via CPM) powers all tests. Create focused cases in `test/*.t.cpp` and tag them when it aids filtering. Extend `test/CMakeLists.txt` with `make_test(<name> <files...>)` for new executables. Ensure both `default` and `release` presets are green before submitting. Test all three scope guard variants: `scope(exit)` for unconditional execution, `scope(success)` for normal exit only, and `scope(failure)` for exception exit only.

## Commit & Pull Request Guidelines
Write commits in short, imperative present tense (`"add unit tests"`, `"adjust scope guard"`). PRs should explain the change, motivation, and reference relevant issues. Always attach evidence of local verification by running `cmake --workflow --preset=default` and `ctest --preset=release`.

## Security & Configuration Tips
Use the provided Ninja presets and GCC/Clang toolchains; do not diverge without discussion. The project supports both GCC (default presets) and Clang (presets with `-clang` suffix) compilation. Preserve the `scope(exit)`, `scope(success)`, and `scope(failure)` contracts and public macro surface unless coordinating a version bump.

## Agent-Specific Instructions
Treat this document as binding guidance; keep edits minimal and scoped. Match the existing style, avoid unrelated churn, and remember that anything under `include/` ships directly to users.
