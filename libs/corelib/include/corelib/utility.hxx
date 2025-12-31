// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_UTILITY_HXX
#define LOGENIUM_CORELIB_UTILITY_HXX

/**
 * @file utility.hxx
 * @brief Convenience header for all utility module components
 *
 * This header includes all utility components: type introspection (type_name),
 * RAII utilities (Defer, AutoRelease). Include this file to get access to all
 * utility features at once.
 */

/**
 * @defgroup utility Utility Module
 * @brief Core utility functions and RAII wrappers
 *
 * The utility module provides essential tools for C++ development including:
 * - **Type introspection**: Compile-time type name extraction using compiler intrinsics
 * - **RAII utilities**: Automatic resource management and scope-exit execution
 * - **Generic wrappers**: Adapters for managing C-style resources with custom cleanup
 *
 * All utilities are designed to be zero-cost abstractions that leverage C++23 features
 * like constexpr evaluation, perfect forwarding, and move semantics.
 *
 * ## Components
 *
 * ### Type Introspection
 * - `type_name<T>()`: Extract type names at compile time for debugging and diagnostics
 *
 * ### RAII Utilities
 * - `Defer`: Execute arbitrary code at scope exit (like Go's defer or D's scope(exit))
 * - `AutoRelease`: Manage resources with custom releasers (file descriptors, handles, etc.)
 *
 * ## Usage
 *
 * Include the convenience header to get all utilities:
 * @code
 * #include <corelib/utility.hxx>
 * @endcode
 *
 * Or include specific headers:
 * @code
 * #include <corelib/utility/type_name.hxx>
 * #include <corelib/utility/defer.hxx>
 * #include <corelib/utility/auto_release.hxx>
 * @endcode
 */

#include <utility>  // IWYU pragma: export

#include <corelib/utility/auto_release.hxx>  // IWYU pragma: export
#include <corelib/utility/defer.hxx>         // IWYU pragma: export
#include <corelib/utility/type_name.hxx>     // IWYU pragma: export

#endif  // LOGENIUM_CORELIB_UTILITY_HXX
