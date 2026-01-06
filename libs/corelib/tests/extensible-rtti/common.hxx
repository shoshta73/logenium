// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_TESTS_EXTENSIBLE_RTTI_COMMON_HXX
#define LOGENIUM_CORELIB_TESTS_EXTENSIBLE_RTTI_COMMON_HXX

#include <corelib/extensible-rtti.hxx>

namespace corelib::rtti {

class TestNode : public Extends<TestNode, Base> {
  public:
    int value;
    TestNode() : value(0) {}
    explicit TestNode(int v) : value(v) {}

  private:
    static char ID;
    void anchor() override {}

    friend class Extends<TestNode, Base>;
};

class TestContainer : public Extends<TestContainer, Base> {
  public:
    int capacity;
    TestContainer() : capacity(0) {}
    explicit TestContainer(int c) : capacity(c) {}

  private:
    static char ID;
    void anchor() override {}

    friend class Extends<TestContainer, Base>;
};

class TestElement : public Extends<TestElement, TestNode> {
  public:
    int index;
    TestElement(int v, int i) : index(i) { value = v; }

  private:
    static char ID;
    void anchor() override {}

    friend class Extends<TestElement, TestNode>;
};

class TestList : public Extends<TestList, TestContainer> {
  public:
    int count;
    TestList(int c, int cnt) : count(cnt) { capacity = c; }

  private:
    static char ID;
    void anchor() override {}

    friend class Extends<TestList, TestContainer>;
};

class TestTree : public Extends<TestTree, TestContainer, TestNode> {
  public:
    int depth;
    TestTree(int c, int v, int d) : depth(d) {
        capacity = c;
        value = v;
    }

  private:
    static char ID;
    void anchor() override {}

    friend class Extends<TestTree, TestContainer, TestNode>;
};

}  // namespace corelib::rtti

#endif  // LOGENIUM_CORELIB_TESTS_EXTENSIBLE_RTTI_COMMON_HXX
