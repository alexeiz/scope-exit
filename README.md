# scope-exit

A header-only C++17 library providing a `scope(exit)` macro for automatic resource cleanup using RAII principles. This library offers a zero-overhead alternative to `BOOST_SCOPE_EXIT` without dynamic memory allocation.

## Features

- **Header-only**: No compilation or linking required
- **C++17 standard**: Modern C++ with perfect forwarding
- **Zero overhead**: No dynamic memory allocation
- **Exception safe**: Cleanup code executes during stack unwinding
- **Simple syntax**: Intuitive `scope(exit)` macro

## Requirements

- C++17 compatible compiler
- CMake 3.28+ (for building tests only)
- No runtime dependencies

## Quick Start

```cpp
#include <scope_exit/scope_exit.hpp>
#include <cstdio>

int main() {
    FILE* f = fopen("example.txt", "r");
    if (!f) return -1;

    // Automatic cleanup on scope exit
    scope(exit) {
        fclose(f);
    };

    // Use file...
    // File automatically closed when leaving scope
    return 0;
}
```

## Usage Examples

### Basic Resource Management

```cpp
#include <scope_exit/scope_exit.hpp>

void example() {
    int* ptr = new int(42);

    scope(exit) {
        delete ptr;
    };

    // Use ptr...
    // Memory automatically freed on scope exit
}
```

### Multiple Scope Guards (LIFO Order)

```cpp
void multiple_guards() {
    scope(exit) { std::cout << "Third\n"; };   // Executes 1st
    scope(exit) { std::cout << "Second\n"; };  // Executes 2nd
    scope(exit) { std::cout << "First\n"; };   // Executes 3rd

    // Output: First, Second, Third
}
```

### Exception Safety

```cpp
void exception_safe() {
    bool cleanup_needed = true;

    scope(exit) {
        if (cleanup_needed) {
            std::cout << "Cleaning up after exception\n";
        }
    };

    throw std::runtime_error("Error!");
    // Cleanup code still executes during stack unwinding
}
```

### Transaction Pattern

```cpp
void transaction_example() {
    bool committed = false;

    scope(exit) {
        if (!committed) {
            rollback_transaction();
        }
    };

    perform_operations();
    commit_transaction();
    committed = true;
    // No rollback needed on successful path
}
```

## Installation

### Method 1: Header-Only (Simplest)

Copy the single header file to your project:

```cpp
#include "scope_exit.hpp"  // Local copy
```

### Method 2: CMake with FetchContent (Recommended)

```cmake
include(FetchContent)

FetchContent_Declare(
    scope_exit
    GIT_REPOSITORY https://github.com/alexeiz/scope-exit.git
    GIT_TAG        master  # or specific version tag
)

FetchContent_MakeAvailable(scope_exit)

# Link to your target
target_link_libraries(your_target PRIVATE scope_exit::scope_exit)
```

### Method 3: System Installation

```bash
git clone https://github.com/alexeiz/scope-exit.git
cd scope-exit
cmake --preset=release
cmake --build --preset=release
sudo cmake --install build
```

Then in your CMake project:

```cmake
find_package(scope_exit REQUIRED)
target_link_libraries(your_target PRIVATE scope_exit::scope_exit)
```

## Building and Testing

### Configure and Build

```bash
# Configure (Debug by default)
cmake --preset=default

# Or configure for release
cmake --preset=release

# Build
cmake --build --preset=default
```

### Run Tests

```bash
# Run all tests
ctest --preset=default

# Or run specific test executable
./build/test/test_scope_exit
```

### Complete Workflow

```bash
# One command: configure + build + test
cmake --workflow --preset=default
```

## API Reference

### `scope(exit)` Macro

```cpp
scope(exit) { /* cleanup code */ };
```

- **Purpose**: Execute arbitrary code when leaving the current scope
- **Execution**: Always executes, even during exception unwinding
- **Order**: Multiple scope guards execute in LIFO (reverse declaration) order
- **Capture**: Lambda-style capture of surrounding variables by reference

## How It Works

The library uses C++ RAII principles:

1. The `scope(exit)` macro creates a lambda and binds it to a scope guard object
2. The scope guard's destructor executes the lambda when the object goes out of scope
3. Uses perfect forwarding and template metaprogramming for zero overhead
4. Employs `__COUNTER__` macro to generate unique variable names

## License

This library is distributed under the [Boost Software License 1.0](LICENSE_1_0.txt).

```
Copyright Alexei Zakharov, 2025.

Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
```

## Contributing

Issues and pull requests are welcome on [GitHub](https://github.com/alexeiz/scope-exit).
