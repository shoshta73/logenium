// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_TESTS_CASTING_COMMON_HXX
#define LOGENIUM_CORELIB_TESTS_CASTING_COMMON_HXX

#include <cstdint>

namespace corelib {

struct TestShape {
    enum ShapeKind : std::uint8_t { SK_Circle, SK_Rectangle, SK_Triangle };

    ShapeKind kind;

    explicit TestShape(ShapeKind k) : kind(k) {}
    virtual ~TestShape() = default;

    TestShape(const TestShape &) = default;
    TestShape &operator=(const TestShape &) = default;
    TestShape(TestShape &&) = default;
    TestShape &operator=(TestShape &&) = default;
};

struct TestCircle : public TestShape {
    int radius;

    explicit TestCircle(int r) : TestShape(SK_Circle), radius(r) {}

    static bool classof(const TestShape *s) { return s->kind == SK_Circle; }
};

struct TestRectangle : public TestShape {
    int width;
    int height;

    TestRectangle(int w, int h) : TestShape(SK_Rectangle), width(w), height(h) {}

    static bool classof(const TestShape *s) { return s->kind == SK_Rectangle; }
};

struct TestTriangle : public TestShape {
    int base;
    int height;

    TestTriangle(int b, int h) : TestShape(SK_Triangle), base(b), height(h) {}

    static bool classof(const TestShape *s) { return s->kind == SK_Triangle; }
};

}  // namespace corelib

#endif  // LOGENIUM_CORELIB_TESTS_CASTING_COMMON_HXX
