// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_TYPES_FLOAT_HXX
#define LOGENIUM_CORELIB_TYPES_FLOAT_HXX

#include <limits>

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

/**
 * @ingroup types
 * @brief Minimum positive normalized value for f32 type
 *
 * Compile-time constant representing the smallest positive normalized value
 * for a 32-bit floating-point number. Note that this is NOT the most negative
 * value, but rather the smallest positive value that can be represented with
 * full precision.
 *
 * @note Value: approximately 1.175494e-38
 * @note For the most negative value, use -f32_max
 * @note For denormalized (subnormal) minimum, see std::numeric_limits<f32>::denorm_min()
 */
constexpr f32 f32_min = std::numeric_limits<f32>::min();

/**
 * @ingroup types
 * @brief Maximum value for f32 type
 *
 * Compile-time constant representing the maximum finite value for a 32-bit
 * floating-point number.
 *
 * @note Value: approximately 3.402823e+38
 */
constexpr f32 f32_max = std::numeric_limits<f32>::max();

/**
 * @ingroup types
 * @brief Minimum positive normalized value for f64 type
 *
 * Compile-time constant representing the smallest positive normalized value
 * for a 64-bit floating-point number. Note that this is NOT the most negative
 * value, but rather the smallest positive value that can be represented with
 * full precision.
 *
 * @note Value: approximately 2.225074e-308
 * @note For the most negative value, use -f64_max
 * @note For denormalized (subnormal) minimum, see std::numeric_limits<f64>::denorm_min()
 */
constexpr f64 f64_min = std::numeric_limits<f64>::min();

/**
 * @ingroup types
 * @brief Maximum value for f64 type
 *
 * Compile-time constant representing the maximum finite value for a 64-bit
 * floating-point number.
 *
 * @note Value: approximately 1.797693e+308
 */
constexpr f64 f64_max = std::numeric_limits<f64>::max();

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}  // namespace logenium

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_TYPES_FLOAT_HXX
