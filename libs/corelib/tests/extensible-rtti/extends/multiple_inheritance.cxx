// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "../common.hxx"

#include <gtest/gtest.h>

#include <corelib/extensible-rtti/extends.hxx>

namespace corelib::rtti {

TEST(ExtendsMultipleInheritanceTest, TypeIDIsUnique) {
    const void *tree_id = TestTree::TypeID();
    const void *node_id = TestNode::TypeID();
    const void *container_id = TestContainer::TypeID();

    EXPECT_NE(tree_id, node_id);
    EXPECT_NE(tree_id, container_id);
}

TEST(ExtendsMultipleInheritanceTest, DynamicTypeIDMatchesStaticTypeID) {
    TestTree tree(100, 42, 3);
    EXPECT_EQ(tree.DynamicTypeID(), TestTree::TypeID());
}

TEST(ExtendsMultipleInheritanceTest, IsATemplateWithExactType) {
    TestTree tree(100, 42, 3);
    EXPECT_TRUE(tree.IsA<TestTree>());
}

TEST(ExtendsMultipleInheritanceTest, IsATemplateWithPrimaryParent) {
    TestTree tree(100, 42, 3);
    EXPECT_TRUE(tree.IsA<TestContainer>());
}

TEST(ExtendsMultipleInheritanceTest, IsATemplateWithSecondaryParent) {
    TestTree tree(100, 42, 3);
    EXPECT_TRUE(tree.IsA<TestNode>());
}

TEST(ExtendsMultipleInheritanceTest, IsATemplateWithBaseType) {
    TestTree tree(100, 42, 3);
    EXPECT_TRUE(tree.IsA<Base>());
}

TEST(ExtendsMultipleInheritanceTest, IsATemplateWithUnrelatedType) {
    TestTree tree(100, 42, 3);
    EXPECT_FALSE(tree.IsA<TestElement>());
    EXPECT_FALSE(tree.IsA<TestList>());
}

TEST(ExtendsMultipleInheritanceTest, IsAPointerWithAllParents) {
    TestTree tree(100, 42, 3);
    EXPECT_TRUE(tree.IsA(TestTree::TypeID()));
    EXPECT_TRUE(tree.IsA(TestContainer::TypeID()));
    EXPECT_TRUE(tree.IsA(TestNode::TypeID()));
    EXPECT_TRUE(tree.IsA(Base::TypeID()));
}

TEST(ExtendsMultipleInheritanceTest, IsAThroughPrimaryParentPointer) {
    TestTree tree(100, 42, 3);
    TestContainer *container = &tree;

    EXPECT_TRUE(container->IsA(TestTree::TypeID()));
    EXPECT_TRUE(container->IsA(TestContainer::TypeID()));
    EXPECT_TRUE(container->IsA(TestNode::TypeID()));
    EXPECT_TRUE(container->IsA(Base::TypeID()));
}

TEST(ExtendsMultipleInheritanceTest, IsAThroughSecondaryParentPointer) {
    TestTree tree(100, 42, 3);
    TestNode *node = &tree;

    EXPECT_TRUE(node->IsA(TestTree::TypeID()));
    EXPECT_TRUE(node->IsA(TestContainer::TypeID()));
    EXPECT_TRUE(node->IsA(TestNode::TypeID()));
    EXPECT_TRUE(node->IsA(Base::TypeID()));
}

TEST(ExtendsMultipleInheritanceTest, ClassofWithCorrectType) {
    TestTree tree(100, 42, 3);

    EXPECT_TRUE(TestTree::classof(&tree));
}

TEST(ExtendsMultipleInheritanceTest, ClassofWithPrimaryParentType) {
    TestTree tree(100, 42, 3);

    EXPECT_TRUE(TestContainer::classof(&tree));
}

TEST(ExtendsMultipleInheritanceTest, ClassofWithSecondaryParentType) {
    TestTree tree(100, 42, 3);

    EXPECT_TRUE(TestNode::classof(&tree));
}

TEST(ExtendsMultipleInheritanceTest, ClassofWithUnrelatedType) {
    TestTree tree(100, 42, 3);

    EXPECT_FALSE(TestElement::classof(&tree));
    EXPECT_FALSE(TestList::classof(&tree));
}

TEST(ExtendsMultipleInheritanceTest, ClassofThroughPrimaryParentPointer) {
    TestTree tree(100, 42, 3);
    TestContainer *container = &tree;

    EXPECT_TRUE(TestTree::classof(container));
    EXPECT_TRUE(TestContainer::classof(container));
    EXPECT_TRUE(TestNode::classof(container));
}

TEST(ExtendsMultipleInheritanceTest, ClassofThroughSecondaryParentPointer) {
    TestTree tree(100, 42, 3);
    TestNode *node = &tree;

    EXPECT_TRUE(TestTree::classof(node));
    EXPECT_TRUE(TestContainer::classof(node));
    EXPECT_TRUE(TestNode::classof(node));
}

}  // namespace corelib::rtti
