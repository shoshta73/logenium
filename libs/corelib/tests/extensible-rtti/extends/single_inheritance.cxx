// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "../common.hxx"

#include <gtest/gtest.h>

#include <corelib/extensible-rtti/extends.hxx>

namespace corelib::rtti {

TEST(ExtendsSingleInheritanceTest, TypeIDIsConsistent) {
    const void *id1 = TestNode::TypeID();
    const void *id2 = TestNode::TypeID();
    EXPECT_EQ(id1, id2);
    EXPECT_NE(id1, nullptr);
}

TEST(ExtendsSingleInheritanceTest, TypeIDIsUnique) {
    const void *node_id = TestNode::TypeID();
    const void *container_id = TestContainer::TypeID();
    const void *element_id = TestElement::TypeID();
    const void *base_id = Base::TypeID();

    EXPECT_NE(node_id, container_id);
    EXPECT_NE(node_id, element_id);
    EXPECT_NE(node_id, base_id);
    EXPECT_NE(container_id, element_id);
}

TEST(ExtendsSingleInheritanceTest, DynamicTypeIDMatchesStaticTypeID) {
    TestNode node(42);
    EXPECT_EQ(node.DynamicTypeID(), TestNode::TypeID());
    EXPECT_NE(node.DynamicTypeID(), Base::TypeID());
}

TEST(ExtendsSingleInheritanceTest, DynamicTypeIDIsDynamic) {
    TestNode node(42);
    TestElement element(10, 0);

    Base *base_ptr1 = &node;
    Base *base_ptr2 = &element;

    EXPECT_EQ(base_ptr1->DynamicTypeID(), TestNode::TypeID());
    EXPECT_EQ(base_ptr2->DynamicTypeID(), TestElement::TypeID());
    EXPECT_NE(base_ptr1->DynamicTypeID(), base_ptr2->DynamicTypeID());
}

TEST(ExtendsSingleInheritanceTest, IsATemplateWithExactType) {
    TestNode node(42);
    EXPECT_TRUE(node.IsA<TestNode>());
    EXPECT_FALSE(node.IsA<TestContainer>());
    EXPECT_FALSE(node.IsA<TestElement>());
}

TEST(ExtendsSingleInheritanceTest, IsATemplateWithBaseType) {
    TestNode node(42);
    EXPECT_TRUE(node.IsA<Base>());
}

TEST(ExtendsSingleInheritanceTest, IsATemplateWithDerivedType) {
    TestElement element(10, 0);
    EXPECT_TRUE(element.IsA<TestElement>());
    EXPECT_TRUE(element.IsA<TestNode>());
    EXPECT_TRUE(element.IsA<Base>());
    EXPECT_FALSE(element.IsA<TestContainer>());
}

TEST(ExtendsSingleInheritanceTest, IsAPointerWithExactType) {
    TestNode node(42);
    EXPECT_TRUE(node.IsA(TestNode::TypeID()));
    EXPECT_FALSE(node.IsA(TestContainer::TypeID()));
}

TEST(ExtendsSingleInheritanceTest, IsAPointerWithBaseType) {
    TestNode node(42);
    EXPECT_TRUE(node.IsA(Base::TypeID()));
}

TEST(ExtendsSingleInheritanceTest, IsAPointerWithDerivedType) {
    TestElement element(10, 0);
    EXPECT_TRUE(element.IsA(TestElement::TypeID()));
    EXPECT_TRUE(element.IsA(TestNode::TypeID()));
    EXPECT_TRUE(element.IsA(Base::TypeID()));
    EXPECT_FALSE(element.IsA(TestContainer::TypeID()));
}

TEST(ExtendsSingleInheritanceTest, IsAThroughBasePointer) {
    TestElement element(10, 0);
    Base *base = &element;

    EXPECT_TRUE(base->IsA(TestElement::TypeID()));
    EXPECT_TRUE(base->IsA(TestNode::TypeID()));
    EXPECT_TRUE(base->IsA(Base::TypeID()));
    EXPECT_FALSE(base->IsA(TestContainer::TypeID()));
}

TEST(ExtendsSingleInheritanceTest, ClassofWithCorrectType) {
    TestNode node(42);

    EXPECT_TRUE(TestNode::classof(&node));
}

TEST(ExtendsSingleInheritanceTest, ClassofWithIncorrectType) {
    TestContainer container(100);

    EXPECT_FALSE(TestNode::classof(&container));
}

TEST(ExtendsSingleInheritanceTest, ClassofWithDerivedType) {
    TestElement element(10, 0);

    EXPECT_TRUE(TestNode::classof(&element));
    EXPECT_TRUE(TestElement::classof(&element));
    EXPECT_FALSE(TestContainer::classof(&element));
}

TEST(ExtendsSingleInheritanceTest, ClassofWithConstPointer) {
    const TestElement element(10, 0);

    EXPECT_TRUE(TestNode::classof(&element));
    EXPECT_TRUE(TestElement::classof(&element));
}

TEST(ExtendsSingleInheritanceTest, ClassofThroughNodePointer) {
    TestElement element(10, 0);
    TestNode *node = &element;

    EXPECT_TRUE(TestNode::classof(node));
    EXPECT_TRUE(TestElement::classof(node));
}

}  // namespace corelib::rtti
