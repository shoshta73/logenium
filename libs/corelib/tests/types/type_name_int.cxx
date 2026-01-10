// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <cstdint>
#include <string_view>

#include <gtest/gtest.h>

#include <corelib/types/int.hxx>
#include <corelib/utility/type_name.hxx>

// Test that unsigned integer type aliases resolve to correct type names
TEST(TypeNameInt, unsigned_types) {
    EXPECT_EQ(corelib::type_name<corelib::u8>(), corelib::type_name<std::uint8_t>());
    EXPECT_EQ(corelib::type_name<corelib::u16>(), corelib::type_name<std::uint16_t>());
    EXPECT_EQ(corelib::type_name<corelib::u32>(), corelib::type_name<std::uint32_t>());
    EXPECT_EQ(corelib::type_name<corelib::u64>(), corelib::type_name<std::uint64_t>());
}

// Test that signed integer type aliases (i* prefix) resolve to correct type names
TEST(TypeNameInt, signed_i_prefix_types) {
    EXPECT_EQ(corelib::type_name<corelib::i8>(), corelib::type_name<std::int8_t>());
    EXPECT_EQ(corelib::type_name<corelib::i16>(), corelib::type_name<std::int16_t>());
    EXPECT_EQ(corelib::type_name<corelib::i32>(), corelib::type_name<std::int32_t>());
    EXPECT_EQ(corelib::type_name<corelib::i64>(), corelib::type_name<std::int64_t>());
}

// Test that signed integer type aliases (s* prefix) resolve to correct type names
TEST(TypeNameInt, signed_s_prefix_types) {
    EXPECT_EQ(corelib::type_name<corelib::s8>(), corelib::type_name<std::int8_t>());
    EXPECT_EQ(corelib::type_name<corelib::s16>(), corelib::type_name<std::int16_t>());
    EXPECT_EQ(corelib::type_name<corelib::s32>(), corelib::type_name<std::int32_t>());
    EXPECT_EQ(corelib::type_name<corelib::s64>(), corelib::type_name<std::int64_t>());
}

// Test that s* and i* aliases are equivalent
TEST(TypeNameInt, s_and_i_prefix_equivalence) {
    EXPECT_EQ(corelib::type_name<corelib::s8>(), corelib::type_name<corelib::i8>());
    EXPECT_EQ(corelib::type_name<corelib::s16>(), corelib::type_name<corelib::i16>());
    EXPECT_EQ(corelib::type_name<corelib::s32>(), corelib::type_name<corelib::i32>());
    EXPECT_EQ(corelib::type_name<corelib::s64>(), corelib::type_name<corelib::i64>());
}

// Test unsigned types with value overload
TEST(TypeNameInt, unsigned_value_overload) {
    corelib::u8 u8_val = 255;
    corelib::u16 u16_val = 65535;
    corelib::u32 u32_val = 4294967295U;
    corelib::u64 u64_val = 18446744073709551615ULL;

    EXPECT_EQ(corelib::type_name(u8_val), corelib::type_name<std::uint8_t>());
    EXPECT_EQ(corelib::type_name(u16_val), corelib::type_name<std::uint16_t>());
    EXPECT_EQ(corelib::type_name(u32_val), corelib::type_name<std::uint32_t>());
    EXPECT_EQ(corelib::type_name(u64_val), corelib::type_name<std::uint64_t>());
}

// Test signed types (i* prefix) with value overload
TEST(TypeNameInt, signed_i_prefix_value_overload) {
    corelib::i8 i8_val = -128;
    corelib::i16 i16_val = -32768;
    corelib::i32 i32_val = -2147483648;
    corelib::i64 i64_val = -9223372036854775807LL;

    EXPECT_EQ(corelib::type_name(i8_val), corelib::type_name<std::int8_t>());
    EXPECT_EQ(corelib::type_name(i16_val), corelib::type_name<std::int16_t>());
    EXPECT_EQ(corelib::type_name(i32_val), corelib::type_name<std::int32_t>());
    EXPECT_EQ(corelib::type_name(i64_val), corelib::type_name<std::int64_t>());
}

// Test signed types (s* prefix) with value overload
TEST(TypeNameInt, signed_s_prefix_value_overload) {
    corelib::s8 s8_val = -128;
    corelib::s16 s16_val = -32768;
    corelib::s32 s32_val = -2147483648;
    corelib::s64 s64_val = -9223372036854775807LL;

    EXPECT_EQ(corelib::type_name(s8_val), corelib::type_name<std::int8_t>());
    EXPECT_EQ(corelib::type_name(s16_val), corelib::type_name<std::int16_t>());
    EXPECT_EQ(corelib::type_name(s32_val), corelib::type_name<std::int32_t>());
    EXPECT_EQ(corelib::type_name(s64_val), corelib::type_name<std::int64_t>());
}

// Test const qualified types
TEST(TypeNameInt, const_qualified_types) {
    EXPECT_EQ(corelib::type_name<const corelib::u8>(), corelib::type_name<const std::uint8_t>());
    EXPECT_EQ(corelib::type_name<const corelib::i32>(), corelib::type_name<const std::int32_t>());
    EXPECT_EQ(corelib::type_name<const corelib::s64>(), corelib::type_name<const std::int64_t>());
}

// Test pointer types
TEST(TypeNameInt, pointer_types) {
    EXPECT_EQ(corelib::type_name<corelib::u8 *>(), corelib::type_name<std::uint8_t *>());
    EXPECT_EQ(corelib::type_name<corelib::i32 *>(), corelib::type_name<std::int32_t *>());
    EXPECT_EQ(corelib::type_name<corelib::s64 *>(), corelib::type_name<std::int64_t *>());
}

// Test reference types
TEST(TypeNameInt, reference_types) {
    EXPECT_EQ(corelib::type_name<corelib::u8 &>(), corelib::type_name<std::uint8_t &>());
    EXPECT_EQ(corelib::type_name<corelib::i32 &>(), corelib::type_name<std::int32_t &>());
    EXPECT_EQ(corelib::type_name<const corelib::s64 &>(), corelib::type_name<const std::int64_t &>());
}

// Test constexpr evaluation with type aliases
TEST(TypeNameInt, constexpr_evaluation) {
    constexpr auto u8_name = corelib::type_name<corelib::u8>();
    constexpr auto i32_name = corelib::type_name<corelib::i32>();
    constexpr auto s64_name = corelib::type_name<corelib::s64>();

    EXPECT_EQ(u8_name, corelib::type_name<std::uint8_t>());
    EXPECT_EQ(i32_name, corelib::type_name<std::int32_t>());
    EXPECT_EQ(s64_name, corelib::type_name<std::int64_t>());
}

// Test that type names are non-empty
TEST(TypeNameInt, non_empty_names) {
    EXPECT_FALSE(corelib::type_name<corelib::u8>().empty());
    EXPECT_FALSE(corelib::type_name<corelib::u16>().empty());
    EXPECT_FALSE(corelib::type_name<corelib::u32>().empty());
    EXPECT_FALSE(corelib::type_name<corelib::u64>().empty());
    EXPECT_FALSE(corelib::type_name<corelib::i8>().empty());
    EXPECT_FALSE(corelib::type_name<corelib::i16>().empty());
    EXPECT_FALSE(corelib::type_name<corelib::i32>().empty());
    EXPECT_FALSE(corelib::type_name<corelib::i64>().empty());
    EXPECT_FALSE(corelib::type_name<corelib::s8>().empty());
    EXPECT_FALSE(corelib::type_name<corelib::s16>().empty());
    EXPECT_FALSE(corelib::type_name<corelib::s32>().empty());
    EXPECT_FALSE(corelib::type_name<corelib::s64>().empty());
}
