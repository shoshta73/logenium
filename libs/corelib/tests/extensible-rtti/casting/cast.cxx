// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "../common.hxx"

#include <gtest/gtest.h>

#include <corelib/casting/cast.hxx>

namespace corelib::rtti {

TEST(CastingCastTest, DowncastReference) {
    TestElement element(10, 5);
    TestNode &node = element;

    TestElement &result = corelib::cast<TestElement>(node);
    EXPECT_EQ(&result, &element);
    EXPECT_EQ(result.value, 10);
    EXPECT_EQ(result.index, 5);
}

TEST(CastingCastTest, DowncastPointer) {
    TestElement element(10, 5);
    TestNode *node = &element;

    TestElement *result = corelib::cast<TestElement>(node);
    EXPECT_EQ(result, &element);
    EXPECT_EQ(result->value, 10);
    EXPECT_EQ(result->index, 5);
}

TEST(CastingCastTest, DowncastConstPointer) {
    const TestElement element(10, 5);
    const TestNode *node = &element;

    const TestElement *result = corelib::cast<TestElement>(node);
    EXPECT_EQ(result, &element);
    EXPECT_EQ(result->value, 10);
}

TEST(CastingCastTest, SameTypeReference) {
    TestNode node(42);
    TestNode &ref = node;

    TestNode &result = corelib::cast<TestNode>(ref);
    EXPECT_EQ(&result, &node);
    EXPECT_EQ(result.value, 42);
}

TEST(CastingCastTest, SameTypePointer) {
    TestNode node(42);
    TestNode *ptr = &node;

    TestNode *result = corelib::cast<TestNode>(ptr);
    EXPECT_EQ(result, &node);
    EXPECT_EQ(result->value, 42);
}

TEST(CastingCastTest, MultipleInheritanceToExactType) {
    TestTree tree(100, 42, 3);
    TestContainer *container = &tree;

    TestTree *result = corelib::cast<TestTree>(container);
    EXPECT_EQ(result, &tree);
    EXPECT_EQ(result->capacity, 100);
    EXPECT_EQ(result->value, 42);
    EXPECT_EQ(result->depth, 3);
}

TEST(CastingCastTest, MultipleInheritanceFromPrimaryParent) {
    TestTree tree(100, 42, 3);
    TestContainer *container = &tree;

    TestTree *result = corelib::cast<TestTree>(container);
    EXPECT_EQ(result, &tree);
}

TEST(CastingCastTest, MultipleInheritanceFromSecondaryParent) {
    TestTree tree(100, 42, 3);
    TestNode *node = &tree;

    TestTree *result = corelib::cast<TestTree>(node);
    EXPECT_EQ(result, &tree);
}

}  // namespace corelib::rtti
