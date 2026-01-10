// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_TYPES_FLOAT_HXX
#define LOGENIUM_CORELIB_TYPES_FLOAT_HXX

namespace corelib {

/**
 * @ingroup types
 * @brief 32-bit floating-point type
 *
 * IEEE 754 single-precision floating-point type with 32 bits. Maps to the
 * standard C++ `float` type.
 *
 * @note Provides approximately 7 decimal digits of precision
 * @note Range: approximately ±3.4e38
 * @note Use this for memory-constrained scenarios or when precision requirements are modest
 */
using f32 = float;

/**
 * @ingroup types
 * @brief 64-bit floating-point type
 *
 * IEEE 754 double-precision floating-point type with 64 bits. Maps to the
 * standard C++ `double` type.
 *
 * @note Provides approximately 15-16 decimal digits of precision
 * @note Range: approximately ±1.7e308
 * @note Use this for general-purpose floating-point calculations requiring higher precision
 */
using f64 = double;

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}  // namespace logenium

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_TYPES_FLOAT_HXX
