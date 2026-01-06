// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "../common.hxx"

#include <algorithm>
#include <vector>

#include <gtest/gtest.h>

#include <corelib/casting/predicates.hxx>

namespace corelib::rtti {

TEST(CastingPredicatesTest, IsaPredBasicUsage) {
    TestNode node(42);
    TestContainer container(100);
    TestElement element(10, 5);

    TestNode *node_ptrs[] = {&node, &element};

    auto is_node = corelib::IsaPred<TestNode>;
    EXPECT_TRUE(is_node(&node));
    EXPECT_FALSE(is_node(&container));
    EXPECT_TRUE(is_node(&element));

    auto count = std::count_if(std::begin(node_ptrs), std::end(node_ptrs), is_node);
    EXPECT_EQ(count, 2);
}

TEST(CastingPredicatesTest, IsaPredWithContainer) {
    TestNode node(42);
    TestContainer container(100);
    TestElement element(10, 5);

    TestContainer *container_ptr = &container;

    auto is_container = corelib::IsaPred<TestContainer>;
    EXPECT_FALSE(is_container(&node));
    EXPECT_TRUE(is_container(&container));
    EXPECT_FALSE(is_container(&element));
    EXPECT_TRUE(is_container(container_ptr));
}

TEST(CastingPredicatesTest, IsaAndPresentPredWithNullptr) {
    TestNode node(42);
    TestNode *null_ptr = nullptr;

    auto is_node = corelib::IsaAndPresentPred<TestNode>;
    EXPECT_TRUE(is_node(&node));
    EXPECT_FALSE(is_node(null_ptr));
}

TEST(CastingPredicatesTest, IsaAndPresentPredFiltering) {
    TestNode node(42);
    TestElement element(10, 5);

    TestNode *objects[] = {&node, nullptr, &element, nullptr};

    auto is_node_and_present = corelib::IsaAndPresentPred<TestNode>;
    auto count = std::count_if(std::begin(objects), std::end(objects), is_node_and_present);
    EXPECT_EQ(count, 2);
}

TEST(CastingPredicatesTest, CastToBasicUsage) {
    TestNode node1(42);
    TestNode node2(100);

    TestNode *objects[] = {&node1, &node2};

    std::vector<TestNode *> nodes;
    std::transform(std::begin(objects), std::end(objects), std::back_inserter(nodes), corelib::CastTo<TestNode>);

    EXPECT_EQ(nodes.size(), 2);
    EXPECT_EQ(nodes[0]->value, 42);
    EXPECT_EQ(nodes[1]->value, 100);
}

TEST(CastingPredicatesTest, DynCastToSuccessfulCast) {
    TestElement element(42, 0);
    TestNode *node_ptr = &element;

    auto dyn_cast_functor = corelib::DynCastTo<TestElement>;
    TestElement *result = dyn_cast_functor(node_ptr);

    EXPECT_NE(result, nullptr);
    EXPECT_EQ(result->value, 42);
}

TEST(CastingPredicatesTest, DynCastToFailedCast) {
    TestNode node(42);
    TestNode *node_ptr = &node;

    auto dyn_cast_functor = corelib::DynCastTo<TestElement>;
    TestElement *result = dyn_cast_functor(node_ptr);

    EXPECT_EQ(result, nullptr);
}

TEST(CastingPredicatesTest, CastIfPresentToWithNullptr) {
    TestNode node(42);
    TestNode *null_ptr = nullptr;

    auto cast_functor = corelib::CastIfPresentTo<TestNode>;

    TestNode *result1 = cast_functor(&node);
    EXPECT_NE(result1, nullptr);
    EXPECT_EQ(result1->value, 42);

    TestNode *result2 = cast_functor(null_ptr);
    EXPECT_EQ(result2, nullptr);
}

TEST(CastingPredicatesTest, DynCastIfPresentToWithNullptr) {
    TestElement element(42, 0);
    TestNode node(10);
    TestNode *null_ptr = nullptr;

    auto dyn_cast_functor = corelib::DynCastIfPresentTo<TestElement>;

    TestElement *result1 = dyn_cast_functor(&element);
    EXPECT_NE(result1, nullptr);
    EXPECT_EQ(result1->value, 42);

    TestElement *result2 = dyn_cast_functor(&node);
    EXPECT_EQ(result2, nullptr);

    TestElement *result3 = dyn_cast_functor(null_ptr);
    EXPECT_EQ(result3, nullptr);
}

TEST(CastingPredicatesTest, MultipleInheritanceWithIsaPred) {
    TestTree tree(100, 42, 3);
    TestNode node(10);

    TestNode *node_ptrs[] = {&tree, &node};
    TestContainer *container_ptrs[] = {&tree};

    auto is_node = corelib::IsaPred<TestNode>;
    auto is_tree = corelib::IsaPred<TestTree>;

    auto node_count = std::count_if(std::begin(node_ptrs), std::end(node_ptrs), is_node);
    auto tree_count = std::count_if(std::begin(container_ptrs), std::end(container_ptrs), is_tree);

    EXPECT_EQ(node_count, 2);
    EXPECT_EQ(tree_count, 1);
}

}  // namespace corelib::rtti
