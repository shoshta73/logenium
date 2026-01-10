// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_MATH_VECTOR2_HXX
#define LOGENIUM_CORELIB_MATH_VECTOR2_HXX

#include <cmath>

#include <debug/assert.hxx>

namespace corelib::math {

/**
 * @ingroup math_vectors
 * @brief Template-based 2D vector class.
 *
 * vec2 represents a two-dimensional vector with components accessible
 * through multiple naming schemes (x/y, r/g, s/t) for different use cases.
 *
 * @tparam T The component type (float, double, int, etc.)
 *
 * @note Most operations are constexpr and can be evaluated at compile time
 * @note Length() and Normalized() are non-constexpr due to std::sqrt
 *
 * Example:
 * @code
 * vec2<float> a{1.0f, 2.0f};
 * vec2<float> b{3.0f, 4.0f};
 * auto sum = a + b;                          // (4, 6)
 * float dot = vec2<float>::Dot(a, b);        // 11
 * vec2<float> unit = a.Normalized();         // (0.447, 0.894)
 * @endcode
 */
template <typename T>
struct vec2 {
    /**
     * @brief Union providing multiple naming schemes for the components.
     *
     * - x, y: Spatial coordinates
     * - r, g: Color (red, green) components
     * - s, t: Texture coordinates
     */
    union {
        struct {
            T x;
            T y;
        };
        struct {
            T r;
            T g;
        };
        struct {
            T s;
            T t;
        };
    };

    /**
     * @brief Default constructor, initializes all components to zero.
     */
    constexpr vec2() : x(T{0}), y(T{0}) {}

    /**
     * @brief Construct from a uniform value.
     * @param val The value for all components
     */
    constexpr vec2(T val) : x(val), y(val) {}

    /**
     * @brief Construct from individual components.
     * @param x The x component
     * @param y The y component
     */
    constexpr vec2(T x, T y) : x(x), y(y) {}

    /**
     * @brief Create a zero vector (0, 0).
     * @return vec2 with all components set to zero
     */
    static constexpr vec2 Zero() { return {T{0}, T{0}}; }

    /**
     * @brief Create a unit vector (1, 1).
     * @return vec2 with all components set to one
     */
    static constexpr vec2 One() { return {T{1}, T{1}}; }

    constexpr vec2 operator+(const vec2 &other) const { return {x + other.x, y + other.y}; }
    constexpr vec2 operator-(const vec2 &other) const { return {x - other.x, y - other.y}; }
    constexpr vec2 operator*(const vec2 &other) const { return {x * other.x, y * other.y}; }
    constexpr vec2 operator/(const vec2 &other) const {
        debug::Assert(other.x != T{0} && other.y != T{0}, "Division by zero");
        return {x / other.x, y / other.y};
    }

    constexpr vec2 &operator+=(const vec2 &other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    constexpr vec2 &operator-=(const vec2 &other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr vec2 operator*(T scalar) const { return {x * scalar, y * scalar}; }
    constexpr vec2 operator/(T scalar) const {
        debug::Assert(scalar != T{0}, "Division by zero");
        return {x / scalar, y / scalar};
    }

    constexpr bool operator==(const vec2 &other) const { return x == other.x && y == other.y; }
    constexpr bool operator!=(const vec2 &other) const { return x != other.x || y != other.y; }

    /**
     * @brief Calculate the squared length of the vector.
     * @return The squared length (x * x + y * y)
     * @note Faster than Length() as it avoids the square root operation
     */
    constexpr T LengthSquared() const { return x * x + y * y; }

    /**
     * @brief Calculate the length (magnitude) of the vector.
     * @return The Euclidean length sqrt(x * x + y * y)
     * @note Non-constexpr due to std::sqrt
     */
    T Length() const { return std::sqrt(LengthSquared()); }

    /**
     * @brief Normalize the vector in-place to unit length.
     * @return Reference to this vector after normalization
     * @note Asserts if the vector has zero length (use debug::Assert)
     */
    vec2 &Normalize() {
        T len = Length();
        debug::Assert(len != T{0}, "Cannot normalize a vector with zero length");
        x /= len;
        y /= len;
        return *this;
    }

    /**
     * @brief Get a normalized copy of this vector.
     * @return A new vec2 normalized to unit length
     * @note Asserts if the vector has zero length (use debug::Assert)
     * @note This method is constexpr
     */
    constexpr vec2 Normalized() const {
        T len = Length();
        debug::Assert(len != T{0}, "Cannot normalize a vector with zero length");
        return {x / len, y / len};
    }

    /**
     * @brief Calculate the dot product of two vectors.
     * @param a The first vector
     * @param b The second vector
     * @return The dot product (a.x * b.x + a.y * b.y)
     */
    static constexpr T Dot(const vec2 &a, const vec2 &b) { return a.x * b.x + a.y * b.y; }
};

}  // namespace corelib::math

#ifdef __LOGENENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}  // namespace logenium

#endif  // __LOGENENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_MATH_VECTOR2_HXX
