// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "../common.hxx"

#include <memory>

#include <gtest/gtest.h>

#include <corelib/casting/isa.hxx>

namespace corelib::rtti {

TEST(CastingIsaTest, BasicTypeChecking) {
    TestNode node(42);
    TestContainer container(100);
    TestElement element(10, 0);

    EXPECT_TRUE(corelib::isa<TestNode>(node));
    EXPECT_FALSE(corelib::isa<TestContainer>(node));
    EXPECT_FALSE(corelib::isa<TestElement>(node));

    EXPECT_FALSE(corelib::isa<TestNode>(container));
    EXPECT_TRUE(corelib::isa<TestContainer>(container));

    EXPECT_TRUE(corelib::isa<TestNode>(element));
    EXPECT_FALSE(corelib::isa<TestContainer>(element));
    EXPECT_TRUE(corelib::isa<TestElement>(element));
}

TEST(CastingIsaTest, DerivedTypeChecking) {
    TestElement element(10, 0);
    TestNode &node = element;

    EXPECT_TRUE(corelib::isa<TestNode>(node));
    EXPECT_TRUE(corelib::isa<TestElement>(node));
    EXPECT_FALSE(corelib::isa<TestContainer>(node));
}

TEST(CastingIsaTest, PointerTypeChecking) {
    TestNode node(42);
    TestElement element(10, 0);

    TestNode *node_ptr1 = &node;
    TestNode *node_ptr2 = &element;

    EXPECT_TRUE(corelib::isa<TestNode>(node_ptr1));
    EXPECT_FALSE(corelib::isa<TestElement>(node_ptr1));

    EXPECT_TRUE(corelib::isa<TestNode>(node_ptr2));
    EXPECT_TRUE(corelib::isa<TestElement>(node_ptr2));
}

TEST(CastingIsaTest, ConstPointerTypeChecking) {
    const TestNode node(42);
    const TestElement element(10, 0);

    const TestNode *node_ptr1 = &node;
    const TestNode *node_ptr2 = &element;

    EXPECT_TRUE(corelib::isa<TestNode>(node_ptr1));
    EXPECT_FALSE(corelib::isa<TestElement>(node_ptr1));

    EXPECT_TRUE(corelib::isa<TestNode>(node_ptr2));
    EXPECT_TRUE(corelib::isa<TestElement>(node_ptr2));
}

TEST(CastingIsaTest, VariadicTypeChecking) {
    TestNode node(42);
    TestContainer container(100);
    TestElement element(10, 0);

    EXPECT_TRUE((corelib::isa<TestNode, TestContainer>(node)));
    EXPECT_TRUE((corelib::isa<TestNode, TestContainer>(container)));
    EXPECT_TRUE((corelib::isa<TestNode, TestContainer, TestElement>(element)));

    EXPECT_FALSE((corelib::isa<TestContainer, TestList>(node)));
}

TEST(CastingIsaTest, UniquePtrTypeChecking) {
    auto node = std::make_unique<TestNode>(42);
    auto container = std::make_unique<TestContainer>(100);

    EXPECT_TRUE(corelib::isa<TestNode>(node));
    EXPECT_FALSE(corelib::isa<TestContainer>(node));

    EXPECT_FALSE(corelib::isa<TestNode>(container));
    EXPECT_TRUE(corelib::isa<TestContainer>(container));
}

TEST(CastingIsaTest, MultipleInheritance) {
    TestTree tree(100, 42, 3);

    EXPECT_TRUE(corelib::isa<TestTree>(tree));
    EXPECT_TRUE(corelib::isa<TestContainer>(tree));
    EXPECT_TRUE(corelib::isa<TestNode>(tree));
    EXPECT_FALSE(corelib::isa<TestElement>(tree));
}

TEST(CastingIsaTest, MultipleInheritanceThroughPointer) {
    TestTree tree(100, 42, 3);
    TestContainer *container = &tree;
    TestNode *node = &tree;

    EXPECT_TRUE(corelib::isa<TestTree>(container));
    EXPECT_TRUE(corelib::isa<TestContainer>(container));
    EXPECT_TRUE(corelib::isa<TestNode>(container));

    EXPECT_TRUE(corelib::isa<TestTree>(node));
    EXPECT_TRUE(corelib::isa<TestContainer>(node));
    EXPECT_TRUE(corelib::isa<TestNode>(node));
}

}  // namespace corelib::rtti
