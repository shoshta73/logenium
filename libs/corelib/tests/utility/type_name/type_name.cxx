// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <string_view>

#include <gtest/gtest.h>

#include <corelib/utility/type_name.hxx>

TEST(TypeName, basic_integer_types) {
    EXPECT_EQ(corelib::type_name<int>(), "int");
    EXPECT_EQ(corelib::type_name<unsigned int>(), "unsigned int");
    EXPECT_EQ(corelib::type_name<short>(), "short");
    EXPECT_EQ(corelib::type_name<unsigned short>(), "unsigned short");
    EXPECT_EQ(corelib::type_name<char>(), "char");
    EXPECT_EQ(corelib::type_name<unsigned char>(), "unsigned char");
    EXPECT_EQ(corelib::type_name<signed char>(), "signed char");
}

TEST(TypeName, long_integer_types) {
    EXPECT_EQ(corelib::type_name<long>(), "long");
    EXPECT_EQ(corelib::type_name<unsigned long>(), "unsigned long");
    EXPECT_EQ(corelib::type_name<long long>(), "long long");
    EXPECT_EQ(corelib::type_name<unsigned long long>(), "unsigned long long");
}

TEST(TypeName, floating_point_types) {
    EXPECT_EQ(corelib::type_name<float>(), "float");
    EXPECT_EQ(corelib::type_name<double>(), "double");
    EXPECT_EQ(corelib::type_name<long double>(), "long double");
}

TEST(TypeName, boolean_type) { EXPECT_EQ(corelib::type_name<bool>(), "bool"); }

TEST(TypeName, void_type) { EXPECT_EQ(corelib::type_name<void>(), "void"); }

TEST(TypeName, pointer_types) {
    EXPECT_EQ(corelib::type_name<int *>(), "int *");
    EXPECT_EQ(corelib::type_name<char *>(), "char *");
    EXPECT_EQ(corelib::type_name<void *>(), "void *");
    EXPECT_EQ(corelib::type_name<double *>(), "double *");
}

TEST(TypeName, const_types) {
    EXPECT_EQ(corelib::type_name<const int>(), "const int");
    EXPECT_EQ(corelib::type_name<const char>(), "const char");
    EXPECT_EQ(corelib::type_name<const int *>(), "const int *");
    EXPECT_EQ(corelib::type_name<int *const>(), "int *const");
    EXPECT_EQ(corelib::type_name<const int *const>(), "const int *const");
}

TEST(TypeName, reference_types) {
    EXPECT_EQ(corelib::type_name<int &>(), "int &");
    EXPECT_EQ(corelib::type_name<const int &>(), "const int &");
    EXPECT_EQ(corelib::type_name<int &&>(), "int &&");
}

TEST(TypeName, custom_types) {
    struct CustomStruct {};
    class CustomClass {};

    auto struct_name = corelib::type_name<CustomStruct>();
    auto class_name = corelib::type_name<CustomClass>();

    EXPECT_TRUE(struct_name.find("CustomStruct") != std::string_view::npos)
        << "Expected 'CustomStruct' in: " << struct_name;
    EXPECT_TRUE(class_name.find("CustomClass") != std::string_view::npos)
        << "Expected 'CustomClass' in: " << class_name;
}

TEST(TypeName, value_overload_basic_types) {
    int i = 42;
    float f = 3.14f;
    double d = 2.718;
    char c = 'A';
    bool b = true;

    EXPECT_EQ(corelib::type_name(i), "int");
    EXPECT_EQ(corelib::type_name(f), "float");
    EXPECT_EQ(corelib::type_name(d), "double");
    EXPECT_EQ(corelib::type_name(c), "char");
    EXPECT_EQ(corelib::type_name(b), "bool");
}

TEST(TypeName, value_overload_pointer_types) {
    int *ptr = nullptr;
    const char *str = "hello";

    EXPECT_EQ(corelib::type_name(ptr), "int *");
    EXPECT_EQ(corelib::type_name(str), "const char *");
}

TEST(TypeName, value_overload_const_types) {
    const int ci = 42;
    const double cd = 3.14;

    EXPECT_EQ(corelib::type_name(ci), "const int");
    EXPECT_EQ(corelib::type_name(cd), "const double");
}

TEST(TypeName, constexpr_evaluation) {
    constexpr auto int_name = corelib::type_name<int>();
    constexpr auto float_name = corelib::type_name<float>();

    static_assert(int_name == "int", "type_name<int>() should be constexpr evaluable");
    static_assert(float_name == "float", "type_name<float>() should be constexpr evaluable");

    EXPECT_EQ(int_name, "int");
    EXPECT_EQ(float_name, "float");
}

TEST(TypeName, array_types) {
    using IntArray = int[10];
    using CharArray = char[5];

    auto int_array_name = corelib::type_name<IntArray>();
    auto char_array_name = corelib::type_name<CharArray>();

    EXPECT_TRUE(int_array_name.find("int") != std::string_view::npos)
        << "Expected 'int' in array type: " << int_array_name;
    EXPECT_TRUE(int_array_name.find("10") != std::string_view::npos)
        << "Expected '10' in array type: " << int_array_name;

    EXPECT_TRUE(char_array_name.find("char") != std::string_view::npos)
        << "Expected 'char' in array type: " << char_array_name;
    EXPECT_TRUE(char_array_name.find('5') != std::string_view::npos)
        << "Expected '5' in array type: " << char_array_name;
}
