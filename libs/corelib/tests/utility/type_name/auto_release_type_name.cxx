// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <string_view>

#include <gtest/gtest.h>

#include <corelib/utility/auto_release.hxx>
#include <corelib/utility/type_name.hxx>

TEST(AutoReleaseTypeName, int_type) {
    corelib::AutoRelease<int> resource(42, [](int) {});

    auto name = corelib::type_name(resource);

    EXPECT_TRUE(name.find("AutoRelease") != std::string_view::npos) << "Expected 'AutoRelease' in: " << name;
    EXPECT_TRUE(name.find("int") != std::string_view::npos) << "Expected 'int' in: " << name;
}

TEST(AutoReleaseTypeName, pointer_type) {
    int *ptr = nullptr;
    corelib::AutoRelease<int *, nullptr> resource(ptr, [](int *) {});

    auto name = corelib::type_name(resource);

    EXPECT_TRUE(name.find("AutoRelease") != std::string_view::npos) << "Expected 'AutoRelease' in: " << name;
    EXPECT_TRUE(name.find("int") != std::string_view::npos) << "Expected 'int' in: " << name;
}

TEST(AutoReleaseTypeName, custom_invalid_value) {
    corelib::AutoRelease<int, -1> resource(42, [](int) {});

    auto name = corelib::type_name(resource);

    EXPECT_TRUE(name.find("AutoRelease") != std::string_view::npos) << "Expected 'AutoRelease' in: " << name;
    EXPECT_TRUE(name.find("int") != std::string_view::npos) << "Expected 'int' in: " << name;
}

TEST(AutoReleaseTypeName, template_overload) {
    using ResourceType = corelib::AutoRelease<int>;

    auto name = corelib::type_name<ResourceType>();

    EXPECT_TRUE(name.find("AutoRelease") != std::string_view::npos) << "Expected 'AutoRelease' in: " << name;
    EXPECT_TRUE(name.find("int") != std::string_view::npos) << "Expected 'int' in: " << name;
}

TEST(AutoReleaseTypeName, const_auto_release) {
    const corelib::AutoRelease<int> resource(42, [](int) {});

    auto name = corelib::type_name(resource);

    EXPECT_TRUE(name.find("AutoRelease") != std::string_view::npos) << "Expected 'AutoRelease' in: " << name;
    EXPECT_TRUE(name.find("const") != std::string_view::npos) << "Expected 'const' in: " << name;
}

TEST(AutoReleaseTypeName, auto_release_reference) {
    corelib::AutoRelease<int> resource(42, [](int) {});
    auto &ref = resource;

    auto name = corelib::type_name(ref);

    EXPECT_TRUE(name.find("AutoRelease") != std::string_view::npos) << "Expected 'AutoRelease' in: " << name;
}

TEST(AutoReleaseTypeName, auto_release_const_reference) {
    corelib::AutoRelease<int> resource(42, [](int) {});
    const auto &ref = resource;

    auto name = corelib::type_name(ref);

    EXPECT_TRUE(name.find("AutoRelease") != std::string_view::npos) << "Expected 'AutoRelease' in: " << name;
    EXPECT_TRUE(name.find("const") != std::string_view::npos) << "Expected 'const' in: " << name;
}

TEST(AutoReleaseTypeName, custom_struct_type) {
    struct FileHandle {
        int fd;
        bool operator==(const FileHandle &other) const { return fd == other.fd; }
        bool operator!=(const FileHandle &other) const { return fd != other.fd; }
    };

    constexpr FileHandle InvalidHandle{-1};

    corelib::AutoRelease<FileHandle, InvalidHandle> resource(FileHandle{42}, [](FileHandle) {});

    auto name = corelib::type_name(resource);

    EXPECT_TRUE(name.find("AutoRelease") != std::string_view::npos) << "Expected 'AutoRelease' in: " << name;
    EXPECT_TRUE(name.find("FileHandle") != std::string_view::npos) << "Expected 'FileHandle' in: " << name;
}

TEST(AutoReleaseTypeName, long_type) {
    corelib::AutoRelease<long> resource(42L, [](long) {});

    auto name = corelib::type_name(resource);

    EXPECT_TRUE(name.find("AutoRelease") != std::string_view::npos) << "Expected 'AutoRelease' in: " << name;
    EXPECT_TRUE(name.find("long") != std::string_view::npos) << "Expected 'long' in: " << name;
}

TEST(AutoReleaseTypeName, double_type) {
    corelib::AutoRelease<double> resource(3.14, [](double) {});

    auto name = corelib::type_name(resource);

    EXPECT_TRUE(name.find("AutoRelease") != std::string_view::npos) << "Expected 'AutoRelease' in: " << name;
    EXPECT_TRUE(name.find("double") != std::string_view::npos) << "Expected 'double' in: " << name;
}
