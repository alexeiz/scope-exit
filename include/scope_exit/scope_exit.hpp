#pragma once

/// Purpose: provide macro to execute an arbitrary code on scope exit.
///
/// Example:
/// ```
///   FILE * f = fopen("file", "r");
///   scope(exit) { close(f); };
/// ```
///
/// Note:
/// Do not use BOOST_SCOPE_EXIT.  It uses std::function internally and therefore allocates memory.

#include <utility>

namespace scope_exit_v1
{
namespace detail
{

template <typename F>
struct scope_guard
{
    scope_guard(F && f)
        : action{f}
    {}

    ~scope_guard() noexcept(false)
    {
        action();
    }

    F action;
};

struct scope_guard_tag {
    template <typename F>
    friend
    auto operator+(scope_guard_tag, F && f)
    {
        return scope_guard<F>(std::forward<F>(f));
    }
};

}  // namespace detail
}  // namespace scope_exit_v1

#define SCOPE_CONCAT2_(X, Y)  X ## Y
#define SCOPE_CONCAT_(X, Y)   SCOPE_CONCAT2_(X, Y)

#define scope(condition)  scope_ ## condition
#define scope_exit        auto const & SCOPE_CONCAT_(scope_guard_obj_, __COUNTER__)         \
                          __attribute__ ((unused)) = scope_exit_v1::detail::scope_guard_tag{} + [&]

// Copyright Alexei Zakharov, 2025.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
