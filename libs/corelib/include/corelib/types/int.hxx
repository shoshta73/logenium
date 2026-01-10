// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_TYPES_INT_HXX
#define LOGENIUM_CORELIB_TYPES_INT_HXX

#include <cstdint>
#include <limits>

namespace corelib {

/**
 * @ingroup types
 * @brief 8-bit unsigned integer type
 *
 * Fixed-width unsigned integer with exactly 8 bits. Maps to std::uint8_t.
 * Range: 0 to 255.
 */
using u8 = std::uint8_t;

/**
 * @ingroup types
 * @brief 16-bit unsigned integer type
 *
 * Fixed-width unsigned integer with exactly 16 bits. Maps to std::uint16_t.
 * Range: 0 to 65,535.
 */
using u16 = std::uint16_t;

/**
 * @ingroup types
 * @brief 32-bit unsigned integer type
 *
 * Fixed-width unsigned integer with exactly 32 bits. Maps to std::uint32_t.
 * Range: 0 to 4,294,967,295.
 */
using u32 = std::uint32_t;

/**
 * @ingroup types
 * @brief 64-bit unsigned integer type
 *
 * Fixed-width unsigned integer with exactly 64 bits. Maps to std::uint64_t.
 * Range: 0 to 18,446,744,073,709,551,615.
 */
using u64 = std::uint64_t;

/**
 * @ingroup types
 * @brief 8-bit signed integer type
 *
 * Fixed-width signed integer with exactly 8 bits. Maps to std::int8_t.
 * Range: -128 to 127.
 */
using i8 = std::int8_t;

/**
 * @ingroup types
 * @brief 16-bit signed integer type
 *
 * Fixed-width signed integer with exactly 16 bits. Maps to std::int16_t.
 * Range: -32,768 to 32,767.
 */
using i16 = std::int16_t;

/**
 * @ingroup types
 * @brief 32-bit signed integer type
 *
 * Fixed-width signed integer with exactly 32 bits. Maps to std::int32_t.
 * Range: -2,147,483,648 to 2,147,483,647.
 */
using i32 = std::int32_t;

/**
 * @ingroup types
 * @brief 64-bit signed integer type
 *
 * Fixed-width signed integer with exactly 64 bits. Maps to std::int64_t.
 * Range: -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807.
 */
using i64 = std::int64_t;

/**
 * @ingroup types
 * @brief 8-bit signed integer type (alias for i8)
 *
 * Alternative name for i8. Provides the 's' (signed) prefix convention
 * as an alternative to the 'i' (integer) prefix.
 */
using s8 = i8;

/**
 * @ingroup types
 * @brief 16-bit signed integer type (alias for i16)
 *
 * Alternative name for i16. Provides the 's' (signed) prefix convention
 * as an alternative to the 'i' (integer) prefix.
 */
using s16 = i16;

/**
 * @ingroup types
 * @brief 32-bit signed integer type (alias for i32)
 *
 * Alternative name for i32. Provides the 's' (signed) prefix convention
 * as an alternative to the 'i' (integer) prefix.
 */
using s32 = i32;

/**
 * @ingroup types
 * @brief 64-bit signed integer type (alias for i64)
 *
 * Alternative name for i64. Provides the 's' (signed) prefix convention
 * as an alternative to the 'i' (integer) prefix.
 */
using s64 = i64;

/**
 * @ingroup types
 * @brief Minimum value for u8 type
 *
 * Compile-time constant representing the minimum value for an 8-bit unsigned integer.
 * Value: 0
 */
constexpr u8 u8_min = std::numeric_limits<u8>::min();

/**
 * @ingroup types
 * @brief Maximum value for u8 type
 *
 * Compile-time constant representing the maximum value for an 8-bit unsigned integer.
 * Value: 255
 */
constexpr u8 u8_max = std::numeric_limits<u8>::max();

/**
 * @ingroup types
 * @brief Minimum value for u16 type
 *
 * Compile-time constant representing the minimum value for a 16-bit unsigned integer.
 * Value: 0
 */
constexpr u16 u16_min = std::numeric_limits<u16>::min();

/**
 * @ingroup types
 * @brief Maximum value for u16 type
 *
 * Compile-time constant representing the maximum value for a 16-bit unsigned integer.
 * Value: 65,535
 */
constexpr u16 u16_max = std::numeric_limits<u16>::max();

/**
 * @ingroup types
 * @brief Minimum value for u32 type
 *
 * Compile-time constant representing the minimum value for a 32-bit unsigned integer.
 * Value: 0
 */
constexpr u32 u32_min = std::numeric_limits<u32>::min();

/**
 * @ingroup types
 * @brief Maximum value for u32 type
 *
 * Compile-time constant representing the maximum value for a 32-bit unsigned integer.
 * Value: 4,294,967,295
 */
constexpr u32 u32_max = std::numeric_limits<u32>::max();

/**
 * @ingroup types
 * @brief Minimum value for u64 type
 *
 * Compile-time constant representing the minimum value for a 64-bit unsigned integer.
 * Value: 0
 */
constexpr u64 u64_min = std::numeric_limits<u64>::min();

/**
 * @ingroup types
 * @brief Maximum value for u64 type
 *
 * Compile-time constant representing the maximum value for a 64-bit unsigned integer.
 * Value: 18,446,744,073,709,551,615
 */
constexpr u64 u64_max = std::numeric_limits<u64>::max();

/**
 * @ingroup types
 * @brief Minimum value for i8 type
 *
 * Compile-time constant representing the minimum value for an 8-bit signed integer.
 * Value: -128
 */
constexpr i8 i8_min = std::numeric_limits<i8>::min();

/**
 * @ingroup types
 * @brief Maximum value for i8 type
 *
 * Compile-time constant representing the maximum value for an 8-bit signed integer.
 * Value: 127
 */
constexpr i8 i8_max = std::numeric_limits<i8>::max();

/**
 * @ingroup types
 * @brief Minimum value for i16 type
 *
 * Compile-time constant representing the minimum value for a 16-bit signed integer.
 * Value: -32,768
 */
constexpr i16 i16_min = std::numeric_limits<i16>::min();

/**
 * @ingroup types
 * @brief Maximum value for i16 type
 *
 * Compile-time constant representing the maximum value for a 16-bit signed integer.
 * Value: 32,767
 */
constexpr i16 i16_max = std::numeric_limits<i16>::max();

/**
 * @ingroup types
 * @brief Minimum value for i32 type
 *
 * Compile-time constant representing the minimum value for a 32-bit signed integer.
 * Value: -2,147,483,648
 */
constexpr i32 i32_min = std::numeric_limits<i32>::min();

/**
 * @ingroup types
 * @brief Maximum value for i32 type
 *
 * Compile-time constant representing the maximum value for a 32-bit signed integer.
 * Value: 2,147,483,647
 */
constexpr i32 i32_max = std::numeric_limits<i32>::max();

/**
 * @ingroup types
 * @brief Minimum value for i64 type
 *
 * Compile-time constant representing the minimum value for a 64-bit signed integer.
 * Value: -9,223,372,036,854,775,808
 */
constexpr i64 i64_min = std::numeric_limits<i64>::min();

/**
 * @ingroup types
 * @brief Maximum value for i64 type
 *
 * Compile-time constant representing the maximum value for a 64-bit signed integer.
 * Value: 9,223,372,036,854,775,807
 */
constexpr i64 i64_max = std::numeric_limits<i64>::max();

/**
 * @ingroup types
 * @brief Minimum value for s8 type
 *
 * Compile-time constant representing the minimum value for an 8-bit signed integer.
 * Alias for i8_min. Value: -128
 */
constexpr s8 s8_min = i8_min;

/**
 * @ingroup types
 * @brief Maximum value for s8 type
 *
 * Compile-time constant representing the maximum value for an 8-bit signed integer.
 * Alias for i8_max. Value: 127
 */
constexpr s8 s8_max = i8_max;

/**
 * @ingroup types
 * @brief Minimum value for s16 type
 *
 * Compile-time constant representing the minimum value for a 16-bit signed integer.
 * Alias for i16_min. Value: -32,768
 */
constexpr s16 s16_min = i16_min;

/**
 * @ingroup types
 * @brief Maximum value for s16 type
 *
 * Compile-time constant representing the maximum value for a 16-bit signed integer.
 * Alias for i16_max. Value: 32,767
 */
constexpr s16 s16_max = i16_max;

/**
 * @ingroup types
 * @brief Minimum value for s32 type
 *
 * Compile-time constant representing the minimum value for a 32-bit signed integer.
 * Alias for i32_min. Value: -2,147,483,648
 */
constexpr s32 s32_min = i32_min;

/**
 * @ingroup types
 * @brief Maximum value for s32 type
 *
 * Compile-time constant representing the maximum value for a 32-bit signed integer.
 * Alias for i32_max. Value: 2,147,483,647
 */
constexpr s32 s32_max = i32_max;

/**
 * @ingroup types
 * @brief Minimum value for s64 type
 *
 * Compile-time constant representing the minimum value for a 64-bit signed integer.
 * Alias for i64_min. Value: -9,223,372,036,854,775,808
 */
constexpr s64 s64_min = i64_min;

/**
 * @ingroup types
 * @brief Maximum value for s64 type
 *
 * Compile-time constant representing the maximum value for a 64-bit signed integer.
 * Alias for i64_max. Value: 9,223,372,036,854,775,807
 */
constexpr s64 s64_max = i64_max;

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}  // namespace logenium

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_TYPES_INT_HXX
