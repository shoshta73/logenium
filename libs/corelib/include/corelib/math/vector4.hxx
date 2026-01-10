// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_MATH_VECTOR4_HXX
#define LOGENIUM_CORELIB_MATH_VECTOR4_HXX

#include <cmath>

#include <debug/assert.hxx>

#include <corelib/internal/tracing.hxx>

namespace corelib::math {

/**
 * @ingroup math_vectors
 * @brief Template-based 4D vector class.
 *
 * Vec4 represents a four-dimensional vector with components accessible
 * through multiple naming schemes (x/y/z/w, r/g/b/a, s/t/p/q) for different use cases.
 *
 * @tparam T The component type (float, double, int, etc.)
 *
 * @note Most operations are constexpr and can be evaluated at compile time
 * @note Length() and Normalized() are non-constexpr due to std::sqrt
 * @note No Cross() product method (cross product is specific to 3D vectors)
 *
 * Example:
 * @code
 * Vec4<float> a{1.0f, 2.0f, 3.0f, 4.0f};
 * Vec4<float> b{5.0f, 6.0f, 7.0f, 8.0f};
 * auto sum = a + b;                         // (6, 8, 10, 12)
 * float dot = Vec4<float>::Dot(a, b);       // 70
 * Vec4<float> unit = a.Normalized();        // (0.182, 0.365, 0.547, 0.730)
 *
 * // RGBA color usage
 * Vec4<float> color{1.0f, 0.5f, 0.25f, 1.0f};
 * color.a = 0.5f;  // Set alpha to 50%
 * @endcode
 */
template <typename T>
struct vec4 {
    /**
     * @brief Union providing multiple naming schemes for the components.
     *
     * - x, y, z, w: Spatial/homogeneous coordinates
     * - r, g, b, a: Color (red, green, blue, alpha) components
     * - s, t, p, q: Texture coordinates
     */
    union {
        struct {
            T x;
            T y;
            T z;
            T w;
        };
        struct {
            T r;
            T g;
            T b;
            T a;
        };
        struct {
            T s;
            T t;
            T p;
            T q;
        };
    };

    /**
     * @brief Default constructor, initializes all components to zero.
     */
    constexpr vec4() : x(T{0}), y(T{0}), z(T{0}), w(T{0}) { CRLB_ZONE_SCOPED; }

    /**
     * @brief Construct from a uniform value.
     * @param val The value for all components
     */
    constexpr vec4(T val) : x(val), y(val), z(val), w(val) { CRLB_ZONE_SCOPED; }

    /**
     * @brief Construct from individual components.
     * @param x The x component
     * @param y The y component
     * @param z The z component
     * @param w The w component
     */
    constexpr vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) { CRLB_ZONE_SCOPED; }

    /**
     * @brief Create a zero vector (0, 0, 0, 0).
     * @return Vec4 with all components set to zero
     */
    static constexpr vec4 Zero() {
        CRLB_ZONE_SCOPED;
        return {T{0}, T{0}, T{0}, T{0}};
    }

    /**
     * @brief Create a unit vector (1, 1, 1, 1).
     * @return Vec4 with all components set to one
     */
    static constexpr vec4 One() { return {T{1}, T{1}, T{1}, T{1}}; }

    constexpr vec4 operator+(const vec4 &other) const {
        CRLB_ZONE_SCOPED;
        return {x + other.x, y + other.y, z + other.z, w + other.w};
    }
    constexpr vec4 operator-(const vec4 &other) const {
        CRLB_ZONE_SCOPED;
        return {x - other.x, y - other.y, z - other.z, w - other.w};
    }
    constexpr vec4 operator*(const vec4 &other) const {
        CRLB_ZONE_SCOPED;
        return {x * other.x, y * other.y, z * other.z, w * other.w};
    }
    constexpr vec4 operator/(const vec4 &other) const {
        CRLB_ZONE_SCOPED;
        debug::Assert(other.x != T{0} && other.y != T{0} && other.z != T{0} && other.w != T{0}, "Division by zero");
        return {x / other.x, y / other.y, z / other.z, w / other.w};
    }

    constexpr vec4 &operator+=(const vec4 &other) {
        CRLB_ZONE_SCOPED;
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }
    constexpr vec4 &operator-=(const vec4 &other) {
        CRLB_ZONE_SCOPED;
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    constexpr vec4 operator*(T scalar) const {
        CRLB_ZONE_SCOPED;
        return {x * scalar, y * scalar, z * scalar, w * scalar};
    }
    constexpr vec4 operator/(T scalar) const {
        CRLB_ZONE_SCOPED;
        debug::Assert(scalar != T{0}, "Division by zero");
        return {x / scalar, y / scalar, z / scalar, w / scalar};
    }

    constexpr bool operator==(const vec4 &other) const {
        CRLB_ZONE_SCOPED;
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }
    constexpr bool operator!=(const vec4 &other) const {
        CRLB_ZONE_SCOPED;
        return x != other.x || y != other.y || z != other.z || w != other.w;
    }

    /**
     * @brief Calculate the squared length of the vector.
     * @return The squared length (x * x + y * y + z * z + w * w)
     * @note Faster than Length() as it avoids the square root operation
     */
    constexpr T LengthSquared() const {
        CRLB_ZONE_SCOPED;
        return x * x + y * y + z * z + w * w;
    }

    /**
     * @brief Calculate the length (magnitude) of the vector.
     * @return The Euclidean length sqrt(x * x + y * y + z * z + w * w)
     * @note Non-constexpr due to std::sqrt
     */
    T Length() const {
        CRLB_ZONE_SCOPED;
        return std::sqrt(LengthSquared());
    }

    /**
     * @brief Normalize the vector in-place to unit length.
     * @return Reference to this vector after normalization
     * @note Asserts if the vector has zero length (use debug::Assert)
     */
    vec4 &Normalize() {
        CRLB_ZONE_SCOPED;
        T len = Length();
        debug::Assert(len != T{0}, "Cannot normalize a vector with zero length");
        x /= len;
        y /= len;
        z /= len;
        w /= len;
        return *this;
    }

    /**
     * @brief Get a normalized copy of this vector.
     * @return A new Vec4 normalized to unit length
     * @note Asserts if the vector has zero length (use debug::Assert)
     * @note This method is constexpr
     */
    constexpr vec4 Normalized() const {
        CRLB_ZONE_SCOPED;
        T len = Length();
        debug::Assert(len != T{0}, "Cannot normalize a vector with zero length");
        return {x / len, y / len, z / len, w / len};
    }

    /**
     * @brief Calculate the dot product of two vectors.
     * @param a The first vector
     * @param b The second vector
     * @return The dot product (a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w)
     */
    static constexpr T Dot(const vec4 &a, const vec4 &b) {
        CRLB_ZONE_SCOPED;
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }
};

}  // namespace corelib::math

#ifdef __LOGENENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}  // namespace logenium

#endif  // __LOGENENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_MATH_VECTOR4_HXX
