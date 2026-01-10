// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_TYPES_INT_HXX
#define LOGENIUM_CORELIB_TYPES_INT_HXX

#include <cstdint>

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

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}  // namespace logenium

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_TYPES_INT_HXX
