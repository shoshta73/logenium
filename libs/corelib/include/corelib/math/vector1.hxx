// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_MATH_VECTOR1_HXX
#define LOGENIUM_CORELIB_MATH_VECTOR1_HXX

#include <cmath>

#include <debug/assert.hxx>

#include <corelib/internal/tracing.hxx>

namespace corelib::math {

/**
 * @ingroup math_vectors
 * @brief Template-based 1D vector class.
 *
 * vec1 represents a one-dimensional vector with a single component accessible
 * through multiple naming schemes (x/r/s) for different use cases.
 *
 * @tparam T The component type (defaults to float)
 *
 * @note Most operations are constexpr and can be evaluated at compile time
 * @note Length() is non-constexpr due to std::abs
 *
 * Example:
 * @code
 * vec1<float> v{5.0f};
 * v += vec1<float>{2.0f};        // v.x == 7.0f
 * float len = v.Length();        // len == 7.0f
 * vec1<float> unit = v.Normalized();  // unit.x == 1.0f
 * @endcode
 */
template <typename T = float>
struct vec1 {
    /**
     * @brief Union providing multiple naming schemes for the component.
     *
     * - x: Spatial coordinate
     * - r: Color (red) component
     * - s: Texture coordinate
     */
    union {
        T x;
        T r;
        T s;
    };

    /**
     * @brief Default constructor, initializes component to zero.
     */
    constexpr vec1() : x(T{0}) { CRLB_ZONE_SCOPED; }

    /**
     * @brief Construct from a single value.
     * @param val The value for the component
     */
    constexpr vec1(T val) : x(val) { CRLB_ZONE_SCOPED; }

    /**
     * @brief Create a zero vector (0).
     * @return vec1 with component set to zero
     */
    static constexpr vec1 Zero() {
        CRLB_ZONE_SCOPED;
        return {T{0}};
    }

    /**
     * @brief Create a unit vector (1).
     * @return vec1 with component set to one
     */
    static constexpr vec1 One() {
        CRLB_ZONE_SCOPED;
        return {T{1}};
    }

    constexpr vec1 operator+(const vec1 &other) const {
        CRLB_ZONE_SCOPED;
        return {x + other.x};
    }

    constexpr vec1 operator-(const vec1 &other) const {
        CRLB_ZONE_SCOPED;
        return {x - other.x};
    }

    constexpr vec1 operator*(const vec1 &other) const {
        CRLB_ZONE_SCOPED;
        return {x * other.x};
    }

    constexpr vec1 operator/(const vec1 &other) const {
        CRLB_ZONE_SCOPED;
        debug::Assert(other.x != T{0}, "Division by zero");
        return {x / other.x};
    }

    constexpr vec1 &operator+=(const vec1 &other) {
        CRLB_ZONE_SCOPED;
        x += other.x;
        return *this;
    }

    constexpr vec1 &operator-=(const vec1 &other) {
        CRLB_ZONE_SCOPED;
        x -= other.x;
        return *this;
    }

    constexpr vec1 operator*(T scalar) const {
        CRLB_ZONE_SCOPED;
        return {x * scalar};
    }

    constexpr vec1 operator/(T scalar) const {
        CRLB_ZONE_SCOPED;
        debug::Assert(scalar != T{0}, "Division by zero");
        return {x / scalar};
    }

    constexpr bool operator==(const vec1 &other) const {
        CRLB_ZONE_SCOPED;
        return x == other.x;
    }

    constexpr bool operator!=(const vec1 &other) const {
        CRLB_ZONE_SCOPED;
        return x != other.x;
    }

    /**
     * @brief Calculate the squared length of the vector.
     * @return The squared length (x * x)
     * @note Faster than Length() as it avoids the square root operation
     */
    constexpr T LengthSquared() const { return x * x; }

    /**
     * @brief Calculate the length (magnitude) of the vector.
     * @return The absolute value of the component
     * @note Uses std::abs for safety with floating-point types
     * @note Non-constexpr due to std::abs
     */
    T Length() const {
        CRLB_ZONE_SCOPED;
        // abs is safer for floating point than sqrt(x*x) for 1D
        return std::abs(x);
    }

    /**
     * @brief Normalize the vector in-place to unit length.
     * @return Reference to this vector after normalization
     * @note For 1D vectors, this effectively computes the sign (-1, 0, or 1)
     * @note Zero vectors remain zero
     */
    vec1 &Normalize() {
        CRLB_ZONE_SCOPED;
        if (x != 0) {
            x = (x > 0) ? T{1} : T{-1};  // Signum essentially
        } else {
            x = T{0};
        }

        return *this;
    }

    /**
     * @brief Get a normalized copy of this vector.
     * @return A new vec1 normalized to unit length
     * @note For 1D vectors, this effectively computes the sign (-1, 0, or 1)
     * @note Zero vectors return zero
     * @note This method is constexpr
     */
    constexpr vec1 Normalized() const {
        CRLB_ZONE_SCOPED;
        if (x != 0) {
            return vec1{(x > 0) ? T{1} : T{-1}};
        } else {
            return vec1{T{0}};
        }
    }

    /**
     * @brief Calculate the dot product of two vectors.
     * @param a The first vector
     * @param b The second vector
     * @return The dot product (a.x * b.x)
     */
    static constexpr T Dot(const vec1 &a, const vec1 &b) {
        CRLB_ZONE_SCOPED;
        return a.x * b.x;
    }
};

}  // namespace corelib::math

#ifdef __LOGENENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}  // namespace logenium

#endif  // __LOGENENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_MATH_VECTOR1_HXX
