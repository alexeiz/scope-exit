# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a header-only C++17 library providing a `scope_exit` macro for automatic resource cleanup. The library implements a scope guard pattern using RAII principles to execute code automatically when leaving a scope.

## Architecture

- **Header-only library**: The entire functionality is in `include/scope_exit/scope_exit.hpp`
- **Namespace**: `scope_exit_v1` contains the implementation details
- **Core components**:
  - `scope_guard<F>` template struct that executes a function on destruction
  - `scope_guard_tag` for operator+ overloading to enable macro syntax
  - `scope(exit)` macro that creates an unnamed scope guard object

## Development Commands

### Configuration and Build
```bash
# Configure with default preset (Debug)
cmake --preset=default

# Configure for release
cmake --preset=release

# Build
cmake --build --preset=default

# Build release
cmake --build --preset=release
```

### Testing
```bash
# Run tests with default preset
ctest --preset=default

# Run tests with release preset
ctest --preset=release

# Run specific test executable directly
./build/test/test_scope_exit
```

### Complete Workflow
```bash
# Full workflow: configure + build + test
cmake --workflow --preset=default
```

## Project Structure

- `include/scope_exit/` - Main header file location
- `include/scope_exit/detail/version.hpp.in` - Version header template (generates versioned header)
- `test/` - Test files using Catch2 framework
- `cmake/` - CMake configuration and dependency management
- `build/` - Build output directory (generated)

## Key Implementation Details

- Uses perfect forwarding and RAII for zero-overhead scope guards
- Employs `__COUNTER__` macro to generate unique variable names
- Destructor is `noexcept(false)` to allow throwing destructors if needed

## Dependencies

- **Build**: CMake 3.28+, C++17 compiler, Ninja
- **Testing**: Catch2 (fetched via CPM)
