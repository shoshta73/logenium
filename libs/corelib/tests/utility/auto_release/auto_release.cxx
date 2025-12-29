// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <stdexcept>
#include <utility>

#include <gtest/gtest.h>

#include <corelib/utility/auto_release.hxx>

TEST(AutoRelease, basic_construction_and_destruction) {
    int release_count = 0;

    {
        corelib::AutoRelease<int> resource(42, [&release_count](int value) {
            release_count++;
            EXPECT_EQ(value, 42);
        });

        EXPECT_EQ(release_count, 0);
    }

    EXPECT_EQ(release_count, 1);
}

TEST(AutoRelease, default_construction_no_release) {
    int release_count = 0;

    {
        corelib::AutoRelease<int> resource;
        EXPECT_EQ(release_count, 0);
    }

    EXPECT_EQ(release_count, 0);
}

TEST(AutoRelease, get_method) {
    corelib::AutoRelease<int> resource(42, [](int) {});

    EXPECT_EQ(resource.Get(), 42);
}

TEST(AutoRelease, conversion_operator) {
    corelib::AutoRelease<int> resource(42, [](int) {});

    int value = resource;
    EXPECT_EQ(value, 42);
}

TEST(AutoRelease, bool_conversion_valid) {
    corelib::AutoRelease<int> resource(42, [](int) {});

    EXPECT_TRUE(static_cast<bool>(resource));
}

TEST(AutoRelease, bool_conversion_invalid) {
    corelib::AutoRelease<int> resource(0, [](int) {});

    EXPECT_FALSE(static_cast<bool>(resource));
}

TEST(AutoRelease, bool_conversion_default_constructed) {
    corelib::AutoRelease<int> resource;

    EXPECT_FALSE(static_cast<bool>(resource));
}

TEST(AutoRelease, dereference_operator) {
    corelib::AutoRelease<int> resource(42, [](int) {});

    EXPECT_EQ(*resource, 42);
}

TEST(AutoRelease, address_of_operator) {
    corelib::AutoRelease<int> resource(42, [](int) {});

    int *ptr = &resource;
    EXPECT_EQ(*ptr, 42);

    *ptr = 100;
    EXPECT_EQ(resource.Get(), 100);
}

TEST(AutoRelease, reset_releases_old_and_stores_new) {
    int release_count = 0;
    int last_released = 0;

    {
        corelib::AutoRelease<int> resource(42, [&](int value) {
            release_count++;
            last_released = value;
        });

        EXPECT_EQ(release_count, 0);

        resource.Reset(100);

        EXPECT_EQ(release_count, 1);
        EXPECT_EQ(last_released, 42);
        EXPECT_EQ(resource.Get(), 100);
    }

    EXPECT_EQ(release_count, 2);
    EXPECT_EQ(last_released, 100);
}

TEST(AutoRelease, reset_with_invalid_value_no_release) {
    int release_count = 0;

    {
        corelib::AutoRelease<int> resource;

        resource.Reset(42);

        EXPECT_EQ(release_count, 0);
        EXPECT_EQ(resource.Get(), 42);
    }

    EXPECT_EQ(release_count, 0);
}

TEST(AutoRelease, move_construction) {
    int release_count = 0;

    {
        corelib::AutoRelease<int> resource1(42, [&release_count](int) { release_count++; });

        corelib::AutoRelease<int> resource2(std::move(resource1));

        EXPECT_EQ(resource2.Get(), 42);
        EXPECT_EQ(release_count, 0);
    }

    EXPECT_EQ(release_count, 1);
}

TEST(AutoRelease, move_assignment) {
    int release_count = 0;

    {
        corelib::AutoRelease<int> resource1(42, [&release_count](int) { release_count++; });
        corelib::AutoRelease<int> resource2(100, [&release_count](int) { release_count++; });

        resource2 = std::move(resource1);

        EXPECT_EQ(release_count, 1);
        EXPECT_EQ(resource2.Get(), 42);
    }

    EXPECT_EQ(release_count, 2);
}

TEST(AutoRelease, swap_functionality) {
    int release_count = 0;

    corelib::AutoRelease<int> resource1(42, [&release_count](int) { release_count++; });
    corelib::AutoRelease<int> resource2(100, [&release_count](int) { release_count++; });

    resource1.Swap(resource2);

    EXPECT_EQ(resource1.Get(), 100);
    EXPECT_EQ(resource2.Get(), 42);
    EXPECT_EQ(release_count, 0);
}

TEST(AutoRelease, custom_invalid_value) {
    int release_count = 0;

    {
        corelib::AutoRelease<int, -1> resource(-1, [&release_count](int) { release_count++; });

        EXPECT_FALSE(static_cast<bool>(resource));
    }

    EXPECT_EQ(release_count, 0);
}

TEST(AutoRelease, custom_invalid_value_valid_resource) {
    int release_count = 0;

    {
        corelib::AutoRelease<int, -1> resource(0, [&release_count](int value) {
            release_count++;
            EXPECT_EQ(value, 0);
        });

        EXPECT_TRUE(static_cast<bool>(resource));
    }

    EXPECT_EQ(release_count, 1);
}

TEST(AutoRelease, pointer_type) {
    int *ptr = new int(42);
    bool released = false;

    {
        corelib::AutoRelease<int *, nullptr> resource(ptr, [&released](const int *p) {
            released = true;
            delete p;
        });

        EXPECT_EQ(*resource.Get(), 42);
        EXPECT_TRUE(static_cast<bool>(resource));
    }

    EXPECT_TRUE(released);
}

TEST(AutoRelease, file_handle_pattern) {
    struct FileHandle {
        int fd;
        bool operator==(const FileHandle &other) const { return fd == other.fd; }
        bool operator!=(const FileHandle &other) const { return fd != other.fd; }
    };

    constexpr FileHandle InvalidHandle{-1};

    bool closed = false;

    {
        corelib::AutoRelease<FileHandle, InvalidHandle> file(FileHandle{42}, [&closed](FileHandle handle) {
            closed = true;
            EXPECT_EQ(handle.fd, 42);
        });

        EXPECT_EQ(file.Get().fd, 42);
        EXPECT_TRUE(static_cast<bool>(file));
    }

    EXPECT_TRUE(closed);
}

TEST(AutoRelease, exception_safety) {
    int release_count = 0;

    try {
        corelib::AutoRelease<int> resource(42, [&release_count](int) { release_count++; });
        throw std::runtime_error("test exception");
    } catch (const std::runtime_error &) {
    }

    EXPECT_EQ(release_count, 1);
}

TEST(AutoRelease, multiple_resources_different_types) {
    int int_release_count = 0;
    int ptr_release_count = 0;

    {
        corelib::AutoRelease<int> int_resource(42, [&int_release_count](int) { int_release_count++; });

        int *ptr = new int(100);
        corelib::AutoRelease<int *, nullptr> ptr_resource(ptr, [&ptr_release_count](const int *p) {
            ptr_release_count++;
            delete p;
        });

        EXPECT_EQ(int_resource.Get(), 42);
        EXPECT_EQ(*ptr_resource.Get(), 100);
    }

    EXPECT_EQ(int_release_count, 1);
    EXPECT_EQ(ptr_release_count, 1);
}

TEST(AutoRelease, reset_to_invalid_value) {
    int release_count = 0;

    {
        corelib::AutoRelease<int> resource(42, [&release_count](int) { release_count++; });

        EXPECT_TRUE(static_cast<bool>(resource));

        resource.Reset(0);

        EXPECT_EQ(release_count, 1);
        EXPECT_FALSE(static_cast<bool>(resource));
    }

    EXPECT_EQ(release_count, 1);
}

TEST(AutoRelease, chained_move_assignment) {
    int release_count = 0;

    {
        corelib::AutoRelease<int> r1(1, [&release_count](int) { release_count++; });
        corelib::AutoRelease<int> r2(2, [&release_count](int) { release_count++; });
        corelib::AutoRelease<int> r3(3, [&release_count](int) { release_count++; });

        r3 = std::move(r2);
        r2 = std::move(r1);

        EXPECT_EQ(r3.Get(), 2);
    }

    EXPECT_EQ(release_count, 3);
}

TEST(AutoRelease, releaser_only_called_once) {
    int release_count = 0;

    {
        corelib::AutoRelease<int> resource(42, [&release_count](int) { release_count++; });
    }

    EXPECT_EQ(release_count, 1);
}

TEST(AutoRelease, move_from_function) {
    int release_count = 0;

    auto create_resource = [&release_count]() {
        return corelib::AutoRelease<int>(42, [&release_count](int) { release_count++; });
    };

    {
        auto resource = create_resource();
        EXPECT_EQ(resource.Get(), 42);
        EXPECT_EQ(release_count, 0);
    }

    EXPECT_EQ(release_count, 1);
}
