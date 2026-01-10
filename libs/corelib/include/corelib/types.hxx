// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_TYPES_HXX
#define LOGENIUM_CORELIB_TYPES_HXX

/**
 * @file types.hxx
 * @brief Convenience header for all types module components
 *
 * This header includes all type alias definitions for integer and floating-point
 * types. Include this file to get access to all type aliases at once.
 */

/**
 * @defgroup types Types Module
 * @brief Platform-independent type aliases for integer and floating-point types
 *
 * The types module provides consistent, portable type aliases that map to standard
 * C++ integer and floating-point types. These aliases use a concise naming convention
 * common in systems programming and game development.
 *
 * ## Integer Types
 *
 * Fixed-width integer types with consistent size across platforms:
 * - **Unsigned**: `u8`, `u16`, `u32`, `u64` (8, 16, 32, 64-bit unsigned integers)
 * - **Signed**: `i8`, `i16`, `i32`, `i64` (8, 16, 32, 64-bit signed integers)
 * - **Signed aliases**: `s8`, `s16`, `s32`, `s64` (alternative names for signed integers)
 *
 * All integer types are defined using the fixed-width types from `<cstdint>`:
 * - `u*` types use `std::uint*_t`
 * - `i*` and `s*` types use `std::int*_t`
 *
 * ## Floating-Point Types
 *
 * IEEE 754 floating-point types with explicit precision:
 * - **f32**: 32-bit single-precision floating-point (maps to `float`)
 * - **f64**: 64-bit double-precision floating-point (maps to `double`)
 *
 * ## Usage
 *
 * Include the convenience header to get all type aliases:
 * @code
 * #include <corelib/types.hxx>
 *
 * u32 unsigned_value = 42;
 * i64 signed_value = -100;
 * f32 float_value = 3.14f;
 * f64 double_value = 2.71828;
 * @endcode
 *
 * Or include specific headers:
 * @code
 * #include <corelib/types/int.hxx>    // Integer types only
 * #include <corelib/types/float.hxx>  // Floating-point types only
 * @endcode
 *
 * ## Rationale
 *
 * These type aliases provide:
 * - **Clarity**: Explicit bit-width in the type name
 * - **Portability**: Consistent behavior across platforms
 * - **Brevity**: Shorter names than standard library equivalents
 * - **Consistency**: Unified naming convention across the codebase
 */

#include <corelib/types/float.hxx>  // IWYU pragma: export
#include <corelib/types/int.hxx>    // IWYU pragma: export

#endif  // LOGENIUM_CORELIB_TYPES_HXX
