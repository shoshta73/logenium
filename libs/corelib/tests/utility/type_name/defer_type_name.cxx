// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <string_view>

#include <gtest/gtest.h>

#include <corelib/utility/defer.hxx>
#include <corelib/utility/type_name.hxx>

TEST(DeferTypeName, lambda_functor) {
    auto lambda = []() {};
    corelib::Defer defer(std::move(lambda));

    auto name = corelib::type_name(defer);

    EXPECT_TRUE(name.find("Defer") != std::string_view::npos) << "Expected 'Defer' in: " << name;
    EXPECT_TRUE(name.find("lambda") != std::string_view::npos) << "Expected 'lambda' in: " << name;
}

TEST(DeferTypeName, lambda_with_capture) {
    int value = 0;
    auto lambda = [&value]() { value = 42; };
    corelib::Defer defer(std::move(lambda));

    auto name = corelib::type_name(defer);

    EXPECT_TRUE(name.find("Defer") != std::string_view::npos) << "Expected 'Defer' in: " << name;
    EXPECT_TRUE(name.find("lambda") != std::string_view::npos) << "Expected 'lambda' in: " << name;
}

TEST(DeferTypeName, mutable_lambda) {
    auto lambda = [counter = 0]() mutable { counter++; };
    corelib::Defer defer(std::move(lambda));

    auto name = corelib::type_name(defer);

    EXPECT_TRUE(name.find("Defer") != std::string_view::npos) << "Expected 'Defer' in: " << name;
    EXPECT_TRUE(name.find("lambda") != std::string_view::npos) << "Expected 'lambda' in: " << name;
}

TEST(DeferTypeName, template_overload) {
    auto lambda = []() {};
    using DeferType = corelib::Defer<decltype(lambda)>;

    auto name = corelib::type_name<DeferType>();

    EXPECT_TRUE(name.find("Defer") != std::string_view::npos) << "Expected 'Defer' in: " << name;
}

TEST(DeferTypeName, const_defer) {
    auto lambda = []() {};
    const corelib::Defer defer(std::move(lambda));

    auto name = corelib::type_name(defer);

    EXPECT_TRUE(name.find("Defer") != std::string_view::npos) << "Expected 'Defer' in: " << name;
    EXPECT_TRUE(name.find("const") != std::string_view::npos) << "Expected 'const' in: " << name;
}

TEST(DeferTypeName, defer_reference) {
    auto lambda = []() {};
    corelib::Defer defer(std::move(lambda));
    auto &ref = defer;

    auto name = corelib::type_name(ref);

    EXPECT_TRUE(name.find("Defer") != std::string_view::npos) << "Expected 'Defer' in: " << name;
}

TEST(DeferTypeName, defer_const_reference) {
    auto lambda = []() {};
    corelib::Defer defer(std::move(lambda));
    const auto &ref = defer;

    auto name = corelib::type_name(ref);

    EXPECT_TRUE(name.find("Defer") != std::string_view::npos) << "Expected 'Defer' in: " << name;
    EXPECT_TRUE(name.find("const") != std::string_view::npos) << "Expected 'const' in: " << name;
}
