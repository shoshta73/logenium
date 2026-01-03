// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "../common.hxx"

#include <memory>

#include <gtest/gtest.h>

#include <corelib/casting/dyn_cast.hxx>

namespace corelib::rtti {

TEST(CastingDynCastTest, SuccessfulDowncastPointer) {
    TestElement element(10, 5);
    TestNode *node = &element;

    TestElement *result = corelib::dyn_cast<TestElement>(node);
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result, &element);
    EXPECT_EQ(result->value, 10);
    EXPECT_EQ(result->index, 5);
}

TEST(CastingDynCastTest, FailedDowncastPointer) {
    TestNode node(42);
    TestNode *node_ptr = &node;

    TestElement *result = corelib::dyn_cast<TestElement>(node_ptr);
    EXPECT_EQ(result, nullptr);
}

TEST(CastingDynCastTest, SuccessfulDowncastConstPointer) {
    const TestElement element(10, 5);
    const TestNode *node = &element;

    const TestElement *result = corelib::dyn_cast<TestElement>(node);
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result, &element);
}

TEST(CastingDynCastTest, FailedDowncastConstPointer) {
    const TestNode node(42);
    const TestNode *node_ptr = &node;

    const TestElement *result = corelib::dyn_cast<TestElement>(node_ptr);
    EXPECT_EQ(result, nullptr);
}

TEST(CastingDynCastTest, SameTypeSuccessfulCast) {
    TestNode node(42);
    TestNode *node_ptr = &node;

    TestNode *result = corelib::dyn_cast<TestNode>(node_ptr);
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result->value, 42);
}

TEST(CastingDynCastTest, DifferentBranchFailedCast) {
    TestElement element(10, 5);
    TestNode *node = &element;

    TestContainer *result = corelib::dyn_cast<TestContainer>(node);
    EXPECT_EQ(result, nullptr);
}

TEST(CastingDynCastTest, MultipleInheritanceSuccessfulCast) {
    TestTree tree(100, 42, 3);
    TestContainer *container = &tree;

    TestTree *result = corelib::dyn_cast<TestTree>(container);
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result->capacity, 100);
    EXPECT_EQ(result->value, 42);
    EXPECT_EQ(result->depth, 3);
}

TEST(CastingDynCastTest, MultipleInheritanceFailedCast) {
    TestList list(100, 5);
    TestContainer *container = &list;

    TestTree *result = corelib::dyn_cast<TestTree>(container);
    EXPECT_EQ(result, nullptr);
}

TEST(CastingDynCastTest, MultipleInheritanceFromPrimaryParent) {
    TestTree tree(100, 42, 3);
    TestContainer *container = &tree;

    TestTree *result = corelib::dyn_cast<TestTree>(container);
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result, &tree);
}

TEST(CastingDynCastTest, MultipleInheritanceFromSecondaryParent) {
    TestTree tree(100, 42, 3);
    TestNode *node = &tree;

    TestTree *result = corelib::dyn_cast<TestTree>(node);
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result, &tree);
}

TEST(CastingDynCastTest, UniquePtrSuccessfulCast) {
    auto node = std::make_unique<TestNode>(42);

    auto result = corelib::unique_dyn_cast<TestNode>(std::move(node));
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result->value, 42);
    EXPECT_EQ(node, nullptr);
}

TEST(CastingDynCastTest, UniquePtrFailedCast) {
    auto node = std::make_unique<TestNode>(42);
    TestNode *original_ptr = node.get();

    auto result = corelib::unique_dyn_cast<TestContainer>(std::move(node));
    EXPECT_EQ(result, nullptr);
    EXPECT_NE(node, nullptr);
    EXPECT_EQ(node.get(), original_ptr);
}

TEST(CastingDynCastTest, UniquePtrHierarchySuccessfulCast) {
    auto element = std::make_unique<TestElement>(10, 5);

    auto result = corelib::unique_dyn_cast<TestNode>(std::move(element));
    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result->value, 10);
    EXPECT_EQ(element, nullptr);
}

}  // namespace corelib::rtti
