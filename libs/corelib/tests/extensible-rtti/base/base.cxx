// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include "../common.hxx"

#include <gtest/gtest.h>

#include <corelib/extensible-rtti/base.hxx>

namespace corelib::rtti {

TEST(BaseTest, TypeIDIsConsistent) {
    const void *id1 = Base::TypeID();
    const void *id2 = Base::TypeID();
    EXPECT_EQ(id1, id2);
    EXPECT_NE(id1, nullptr);
}

TEST(BaseTest, TypeIDIsUnique) {
    const void *base_id = Base::TypeID();
    const void *node_id = TestNode::TypeID();
    const void *container_id = TestContainer::TypeID();

    EXPECT_NE(base_id, node_id);
    EXPECT_NE(base_id, container_id);
    EXPECT_NE(node_id, container_id);
}

TEST(BaseTest, IsAWithBaseTypeID) {
    TestNode node(42);
    Base &base = node;

    EXPECT_TRUE(base.IsA(Base::TypeID()));
    EXPECT_TRUE(base.IsA(TestNode::TypeID()));
    EXPECT_FALSE(base.IsA(TestContainer::TypeID()));
}

TEST(BaseTest, IsAWithDerivedTypes) {
    TestElement element(10, 0);
    Base &base = element;

    EXPECT_TRUE(base.IsA(Base::TypeID()));
    EXPECT_TRUE(base.IsA(TestNode::TypeID()));
    EXPECT_TRUE(base.IsA(TestElement::TypeID()));
    EXPECT_FALSE(base.IsA(TestContainer::TypeID()));
}

}  // namespace corelib::rtti
