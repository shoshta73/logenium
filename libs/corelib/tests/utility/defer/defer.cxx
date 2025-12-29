// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <stdexcept>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <corelib/utility/defer.hxx>

TEST(Defer, basic_execution) {
    int value = 0;

    {
        corelib::Defer defer([&value]() { value = 42; });
        EXPECT_EQ(value, 0);
    }

    EXPECT_EQ(value, 42);
}

TEST(Defer, multiple_defers_lifo_order) {
    std::vector<int> execution_order;

    {
        corelib::Defer defer1([&execution_order]() { execution_order.push_back(1); });
        corelib::Defer defer2([&execution_order]() { execution_order.push_back(2); });
        corelib::Defer defer3([&execution_order]() { execution_order.push_back(3); });

        EXPECT_TRUE(execution_order.empty());
    }

    ASSERT_EQ(execution_order.size(), 3);
    EXPECT_EQ(execution_order[0], 3);
    EXPECT_EQ(execution_order[1], 2);
    EXPECT_EQ(execution_order[2], 1);
}

TEST(Defer, lambda_with_captures) {
    int counter = 0;
    std::string message;

    {
        corelib::Defer defer([&counter, &message]() {
            counter += 10;
            message = "deferred";
        });

        counter = 5;
        message = "active";

        EXPECT_EQ(counter, 5);
        EXPECT_EQ(message, "active");
    }

    EXPECT_EQ(counter, 15);
    EXPECT_EQ(message, "deferred");
}

TEST(Defer, exception_safety) {
    bool defer_executed = false;

    try {
        corelib::Defer defer([&defer_executed]() { defer_executed = true; });
        throw std::runtime_error("test exception");
    } catch (const std::runtime_error &) {
    }

    EXPECT_TRUE(defer_executed);
}

TEST(Defer, move_semantics) {
    int value = 0;

    auto create_defer = [&value]() { return corelib::Defer([&value]() { value = 100; }); };

    {
        auto defer = create_defer();
        EXPECT_EQ(value, 0);
    }

    EXPECT_EQ(value, 100);
}

TEST(Defer, mutable_lambda) {
    int value = 0;

    {
        corelib::Defer defer([&value, counter = 0]() mutable {
            counter++;
            value = counter;
        });

        EXPECT_EQ(value, 0);
    }

    EXPECT_EQ(value, 1);
}

TEST(Defer, function_pointer) {
    static int global_value = 0;

    auto cleanup = []() { global_value = 42; };

    {
        corelib::Defer defer(std::move(cleanup));
        EXPECT_EQ(global_value, 0);
    }

    EXPECT_EQ(global_value, 42);
}

TEST(Defer, nested_scopes) {
    std::vector<int> values;

    {
        corelib::Defer outer([&values]() { values.push_back(1); });

        {
            corelib::Defer inner([&values]() { values.push_back(2); });
            EXPECT_TRUE(values.empty());
        }

        ASSERT_EQ(values.size(), 1);
        EXPECT_EQ(values[0], 2);
    }

    ASSERT_EQ(values.size(), 2);
    EXPECT_EQ(values[0], 2);
    EXPECT_EQ(values[1], 1);
}

TEST(Defer, resource_cleanup_pattern) {
    bool resource_opened = false;
    bool resource_closed = false;

    {
        resource_opened = true;
        corelib::Defer defer([&resource_closed]() { resource_closed = true; });

        EXPECT_TRUE(resource_opened);
        EXPECT_FALSE(resource_closed);
    }

    EXPECT_TRUE(resource_opened);
    EXPECT_TRUE(resource_closed);
}

TEST(Defer, complex_cleanup_operations) {
    struct Resource {
        int *data;
        bool *destroyed;

        Resource(bool *d) : data(new int(42)), destroyed(d) {}

        ~Resource() {
            if (destroyed) {
                *destroyed = true;
            }
        }
    };

    bool resource_destroyed = false;
    int *leaked_data = nullptr;

    {
        Resource res(&resource_destroyed);
        leaked_data = res.data;

        corelib::Defer defer([data = res.data]() { delete data; });

        EXPECT_FALSE(resource_destroyed);
    }

    EXPECT_TRUE(resource_destroyed);
}
