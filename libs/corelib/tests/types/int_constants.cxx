// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <corelib/types/int.hxx>

#include <cstdint>
#include <limits>

#include <gtest/gtest.h>

// Test unsigned 8-bit integer constants
TEST(IntConstants, u8_constants) {
    EXPECT_EQ(corelib::u8_min, 0);
    EXPECT_EQ(corelib::u8_max, 255);
    EXPECT_EQ(corelib::u8_min, std::numeric_limits<std::uint8_t>::min());
    EXPECT_EQ(corelib::u8_max, std::numeric_limits<std::uint8_t>::max());
}

// Test unsigned 16-bit integer constants
TEST(IntConstants, u16_constants) {
    EXPECT_EQ(corelib::u16_min, 0);
    EXPECT_EQ(corelib::u16_max, 65535);
    EXPECT_EQ(corelib::u16_min, std::numeric_limits<std::uint16_t>::min());
    EXPECT_EQ(corelib::u16_max, std::numeric_limits<std::uint16_t>::max());
}

// Test unsigned 32-bit integer constants
TEST(IntConstants, u32_constants) {
    EXPECT_EQ(corelib::u32_min, 0U);
    EXPECT_EQ(corelib::u32_max, 4294967295U);
    EXPECT_EQ(corelib::u32_min, std::numeric_limits<std::uint32_t>::min());
    EXPECT_EQ(corelib::u32_max, std::numeric_limits<std::uint32_t>::max());
}

// Test unsigned 64-bit integer constants
TEST(IntConstants, u64_constants) {
    EXPECT_EQ(corelib::u64_min, 0ULL);
    EXPECT_EQ(corelib::u64_max, 18446744073709551615ULL);
    EXPECT_EQ(corelib::u64_min, std::numeric_limits<std::uint64_t>::min());
    EXPECT_EQ(corelib::u64_max, std::numeric_limits<std::uint64_t>::max());
}

// Test signed 8-bit integer constants
TEST(IntConstants, i8_constants) {
    EXPECT_EQ(corelib::i8_min, -128);
    EXPECT_EQ(corelib::i8_max, 127);
    EXPECT_EQ(corelib::i8_min, std::numeric_limits<std::int8_t>::min());
    EXPECT_EQ(corelib::i8_max, std::numeric_limits<std::int8_t>::max());
}

// Test signed 16-bit integer constants
TEST(IntConstants, i16_constants) {
    EXPECT_EQ(corelib::i16_min, -32768);
    EXPECT_EQ(corelib::i16_max, 32767);
    EXPECT_EQ(corelib::i16_min, std::numeric_limits<std::int16_t>::min());
    EXPECT_EQ(corelib::i16_max, std::numeric_limits<std::int16_t>::max());
}

// Test signed 32-bit integer constants
TEST(IntConstants, i32_constants) {
    EXPECT_EQ(corelib::i32_min, -2147483648);
    EXPECT_EQ(corelib::i32_max, 2147483647);
    EXPECT_EQ(corelib::i32_min, std::numeric_limits<std::int32_t>::min());
    EXPECT_EQ(corelib::i32_max, std::numeric_limits<std::int32_t>::max());
}

// Test signed 64-bit integer constants
TEST(IntConstants, i64_constants) {
    EXPECT_EQ(corelib::i64_min, -9223372036854775807LL - 1);
    EXPECT_EQ(corelib::i64_max, 9223372036854775807LL);
    EXPECT_EQ(corelib::i64_min, std::numeric_limits<std::int64_t>::min());
    EXPECT_EQ(corelib::i64_max, std::numeric_limits<std::int64_t>::max());
}

// Test s8 constants (aliases for i8)
TEST(IntConstants, s8_constants) {
    EXPECT_EQ(corelib::s8_min, -128);
    EXPECT_EQ(corelib::s8_max, 127);
    EXPECT_EQ(corelib::s8_min, corelib::i8_min);
    EXPECT_EQ(corelib::s8_max, corelib::i8_max);
    EXPECT_EQ(corelib::s8_min, std::numeric_limits<std::int8_t>::min());
    EXPECT_EQ(corelib::s8_max, std::numeric_limits<std::int8_t>::max());
}

// Test s16 constants (aliases for i16)
TEST(IntConstants, s16_constants) {
    EXPECT_EQ(corelib::s16_min, -32768);
    EXPECT_EQ(corelib::s16_max, 32767);
    EXPECT_EQ(corelib::s16_min, corelib::i16_min);
    EXPECT_EQ(corelib::s16_max, corelib::i16_max);
    EXPECT_EQ(corelib::s16_min, std::numeric_limits<std::int16_t>::min());
    EXPECT_EQ(corelib::s16_max, std::numeric_limits<std::int16_t>::max());
}

// Test s32 constants (aliases for i32)
TEST(IntConstants, s32_constants) {
    EXPECT_EQ(corelib::s32_min, -2147483648);
    EXPECT_EQ(corelib::s32_max, 2147483647);
    EXPECT_EQ(corelib::s32_min, corelib::i32_min);
    EXPECT_EQ(corelib::s32_max, corelib::i32_max);
    EXPECT_EQ(corelib::s32_min, std::numeric_limits<std::int32_t>::min());
    EXPECT_EQ(corelib::s32_max, std::numeric_limits<std::int32_t>::max());
}

// Test s64 constants (aliases for i64)
TEST(IntConstants, s64_constants) {
    EXPECT_EQ(corelib::s64_min, -9223372036854775807LL - 1);
    EXPECT_EQ(corelib::s64_max, 9223372036854775807LL);
    EXPECT_EQ(corelib::s64_min, corelib::i64_min);
    EXPECT_EQ(corelib::s64_max, corelib::i64_max);
    EXPECT_EQ(corelib::s64_min, std::numeric_limits<std::int64_t>::min());
    EXPECT_EQ(corelib::s64_max, std::numeric_limits<std::int64_t>::max());
}

// Test that constants are compile-time constexpr
TEST(IntConstants, constexpr_evaluation) {
    // These tests verify that constants can be used in constexpr context
    constexpr auto test_u8_min = corelib::u8_min;
    constexpr auto test_u8_max = corelib::u8_max;
    constexpr auto test_u16_min = corelib::u16_min;
    constexpr auto test_u16_max = corelib::u16_max;
    constexpr auto test_u32_min = corelib::u32_min;
    constexpr auto test_u32_max = corelib::u32_max;
    constexpr auto test_u64_min = corelib::u64_min;
    constexpr auto test_u64_max = corelib::u64_max;

    constexpr auto test_i8_min = corelib::i8_min;
    constexpr auto test_i8_max = corelib::i8_max;
    constexpr auto test_i16_min = corelib::i16_min;
    constexpr auto test_i16_max = corelib::i16_max;
    constexpr auto test_i32_min = corelib::i32_min;
    constexpr auto test_i32_max = corelib::i32_max;
    constexpr auto test_i64_min = corelib::i64_min;
    constexpr auto test_i64_max = corelib::i64_max;

    constexpr auto test_s8_min = corelib::s8_min;
    constexpr auto test_s8_max = corelib::s8_max;
    constexpr auto test_s16_min = corelib::s16_min;
    constexpr auto test_s16_max = corelib::s16_max;
    constexpr auto test_s32_min = corelib::s32_min;
    constexpr auto test_s32_max = corelib::s32_max;
    constexpr auto test_s64_min = corelib::s64_min;
    constexpr auto test_s64_max = corelib::s64_max;

    // Verify that the constexpr values match the runtime values
    EXPECT_EQ(test_u8_min, corelib::u8_min);
    EXPECT_EQ(test_u8_max, corelib::u8_max);
    EXPECT_EQ(test_i8_min, corelib::i8_min);
    EXPECT_EQ(test_i8_max, corelib::i8_max);
    EXPECT_EQ(test_s8_min, corelib::s8_min);
    EXPECT_EQ(test_s8_max, corelib::s8_max);
}

// Test unsigned constant ranges
TEST(IntConstants, unsigned_ranges) {
    // Verify that min is always 0 for unsigned types
    EXPECT_EQ(corelib::u8_min, 0);
    EXPECT_EQ(corelib::u16_min, 0);
    EXPECT_EQ(corelib::u32_min, 0);
    EXPECT_EQ(corelib::u64_min, 0);

    // Verify that max values follow the pattern 2^n - 1
    EXPECT_EQ(corelib::u8_max, (1 << 8) - 1);
    EXPECT_EQ(corelib::u16_max, (1 << 16) - 1);
    EXPECT_EQ(corelib::u32_max, (1ULL << 32) - 1);
}

// Test signed constant ranges
TEST(IntConstants, signed_ranges) {
    // Verify that min values follow the pattern -2^(n-1)
    EXPECT_EQ(corelib::i8_min, -(1 << 7));
    EXPECT_EQ(corelib::i16_min, -(1 << 15));
    EXPECT_EQ(corelib::i32_min, -(1LL << 31));

    // Verify that max values follow the pattern 2^(n-1) - 1
    EXPECT_EQ(corelib::i8_max, (1 << 7) - 1);
    EXPECT_EQ(corelib::i16_max, (1 << 15) - 1);
    EXPECT_EQ(corelib::i32_max, (1LL << 31) - 1);
}

// Test that constants can be used in arithmetic
TEST(IntConstants, arithmetic_operations) {
    // Test unsigned arithmetic
    EXPECT_EQ(corelib::u8_max + 1, 256);
    EXPECT_EQ(corelib::u16_max + 1, 65536);

    // Test signed arithmetic
    EXPECT_EQ(corelib::i8_max + 1, 128);
    EXPECT_EQ(corelib::i16_max + 1, 32768);

    // Test range calculations
    EXPECT_EQ(corelib::u8_max - corelib::u8_min, 255);
    EXPECT_EQ(corelib::i8_max - corelib::i8_min, 255);
}
