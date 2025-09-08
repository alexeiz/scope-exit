# scope-exit

A header-only C++17 library providing `scope(exit)`, `scope(success)`, and `scope(failure)` macros for automatic resource cleanup using RAII principles. This library offers a zero-overhead alternative to `BOOST_SCOPE_EXIT` without dynamic memory allocation.

## Features

- **Header-only**: No compilation or linking required
- **C++17 standard**: Modern C++ with perfect forwarding
- **Zero overhead**: No dynamic memory allocation
- **Exception safe**: Cleanup code executes during stack unwinding
- **Multiple scope types**: `scope(exit)`, `scope(success)`, and `scope(failure)` macros
- **Simple syntax**: Intuitive macro-based API

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

### Success/Failure Conditional Execution

```cpp
#include <scope_exit/scope_exit.hpp>

void conditional_cleanup_example() {
    scope(success) {
        std::cout << "Operation completed successfully!\n";
        cleanup_temporary_files();
    };

    scope(failure) {
        std::cout << "Operation failed, performing error recovery\n";
        log_error_details();
        notify_administrators();
    };

    scope(exit) {
        std::cout << "Always executed regardless of success/failure\n";
    };

    // Perform some operation that might throw
    risky_operation();
}
```

### Database Transaction with Success/Failure Guards

```cpp
void database_transaction() {
    auto transaction = db.begin_transaction();

    scope(success) {
        transaction.commit();
        std::cout << "Transaction committed successfully\n";
    };

    scope(failure) {
        transaction.rollback();
        std::cout << "Transaction rolled back due to error\n";
    };

    // Perform database operations
    db.insert_record(record1);
    db.update_record(record2);
    db.delete_record(record3);
    // If any operation throws, failure guard executes
    // If all succeed, success guard executes
}
```

### File Processing with Error Recovery

```cpp
void process_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("Cannot open file");

    std::string backup_filename = filename + ".backup";
    bool backup_created = false;

    scope(success) {
        // Remove backup file on successful processing
        if (backup_created) {
            std::remove(backup_filename.c_str());
        }
        std::cout << "File processed successfully\n";
    };

    scope(failure) {
        // Restore from backup on failure
        if (backup_created) {
            std::rename(backup_filename.c_str(), filename.c_str());
            std::cout << "Restored original file from backup\n";
        }
    };

    // Create backup
    std::ofstream backup(backup_filename);
    backup << file.rdbuf();
    backup_created = true;

    // Process the file (might throw)
    transform_file_content(filename);
}
```

### Mixed Scope Guards in Complex Operations

```cpp
void complex_operation() {
    Resource* resource = acquire_resource();
    bool operation_started = false;

    // Always cleanup resource
    scope(exit) {
        release_resource(resource);
    };

    // Log success metrics
    scope(success) {
        increment_success_counter();
        log_operation_success();
    };

    // Handle failures
    scope(failure) {
        increment_failure_counter();
        log_operation_failure();
        if (operation_started) {
            perform_emergency_cleanup();
        }
    };

    operation_started = true;
    perform_critical_operation(resource);
    // Success/failure guards execute based on whether an exception was thrown
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

### `scope(success)` Macro

```cpp
scope(success) { /* success code */ };
```

- **Purpose**: Execute code only when scope exits normally (without exception)
- **Execution**: Only executes if no exception is thrown during scope lifetime
- **Use cases**: Commit transactions, cleanup temporary resources, success logging
- **Order**: Multiple scope guards execute in LIFO (reverse declaration) order
- **Capture**: Lambda-style capture of surrounding variables by reference

### `scope(failure)` Macro

```cpp
scope(failure) { /* failure code */ };
```

- **Purpose**: Execute code only when scope exits due to exception
- **Execution**: Only executes if an exception is thrown during scope lifetime
- **Use cases**: Rollback transactions, error logging, recovery operations
- **Order**: Multiple scope guards execute in LIFO (reverse declaration) order
- **Capture**: Lambda-style capture of surrounding variables by reference

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
