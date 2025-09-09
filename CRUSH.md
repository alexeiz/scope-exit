# CRUSH - scope_exit Library

## Build Commands
```bash
# Configure and build (Debug)
cmake --preset=default && cmake --build --preset=default

# Release build
cmake --preset=release && cmake --build --preset=release

# Full workflow (configure + build + test)
cmake --workflow --preset=default
```

## Test Commands
```bash
# Run all tests
ctest --preset=default

# Run single test directly
./build/test/test_scope_exit
```

## Code Style
- **C++17**, header-only library design
- **4-space indentation**, no tabs, 120 column limit (`.clang-format`)
- **Braces**: Allman style (after classes, functions, namespaces)
- **Naming**: `lower_snake_case` for files/functions/variables, `UPPER_SNAKE_CASE` for macros
- **Namespace**: Use versioned namespaces like `scope_exit_v1`
- **Headers**: Standard library first, keep `#pragma once`
- **Types**: Middle pointer alignment (`int * ptr`), template perfect forwarding
- **Error handling**: `noexcept(false)` destructors when needed for throwing
- **Tests**: Catch2 v3 framework, `*.t.cpp` files in `test/`

## Dependencies
- CMake 3.28+, Ninja, GCC toolchain
- Catch2 (auto-fetched via CPM)
