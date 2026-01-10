// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_MATH_VECTOR3_HXX
#define LOGENIUM_CORELIB_MATH_VECTOR3_HXX

#include <cmath>

#include <debug/assert.hxx>

namespace corelib::math {

/**
 * @ingroup math_vectors
 * @brief Template-based 3D vector class.
 *
 * Vec3 represents a three-dimensional vector with components accessible
 * through multiple naming schemes (x/y/z, r/g/b, s/t/p) for different use cases.
 *
 * @tparam T The component type (float, double, int, etc.)
 *
 * @note Most operations are constexpr and can be evaluated at compile time
 * @note Length() and Normalized() are non-constexpr due to std::sqrt
 * @note Provides Cross() product method, unique to 3D vectors
 *
 * Example:
 * @code
 * Vec3<float> a{1.0f, 2.0f, 3.0f};
 * Vec3<float> b{4.0f, 5.0f, 6.0f};
 * auto sum = a + b;                            // (5, 7, 9)
 * float dot = Vec3<float>::Dot(a, b);          // 32
 * Vec3<float> cross = Vec3<float>::Cross(a, b); // (-3, 6, -3)
 * Vec3<float> unit = a.Normalized();           // (0.267, 0.535, 0.802)
 * @endcode
 */
template <typename T>
struct Vec3 {
    /**
     * @brief Union providing multiple naming schemes for the components.
     *
     * - x, y, z: Spatial coordinates
     * - r, g, b: Color (red, green, blue) components
     * - s, t, p: Texture coordinates
     */
    union {
        struct {
            T x;
            T y;
            T z;
        };
        struct {
            T r;
            T g;
            T b;
        };
        struct {
            T s;
            T t;
            T p;
        };
    };

    /**
     * @brief Default constructor, initializes all components to zero.
     */
    constexpr Vec3() : x(T{0}), y(T{0}), z(T{0}) {}

    /**
     * @brief Construct from a uniform value.
     * @param val The value for all components
     */
    constexpr Vec3(T val) : x(val), y(val), z(val) {}

    /**
     * @brief Construct from individual components.
     * @param x The x component
     * @param y The y component
     * @param z The z component
     */
    constexpr Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

    /**
     * @brief Create a zero vector (0, 0, 0).
     * @return Vec3 with all components set to zero
     */
    static constexpr Vec3 Zero() { return {T{0}, T{0}, T{0}}; }

    /**
     * @brief Create a unit vector (1, 1, 1).
     * @return Vec3 with all components set to one
     */
    static constexpr Vec3 One() { return {T{1}, T{1}, T{1}}; }

    constexpr Vec3 operator+(const Vec3 &other) const { return {x + other.x, y + other.y, z + other.z}; }
    constexpr Vec3 operator-(const Vec3 &other) const { return {x - other.x, y - other.y, z - other.z}; }
    constexpr Vec3 operator*(const Vec3 &other) const { return {x * other.x, y * other.y, z * other.z}; }
    constexpr Vec3 operator/(const Vec3 &other) const {
        debug::Assert(other.x != T{0} && other.y != T{0} && other.z != T{0}, "Division by zero");
        return {x / other.x, y / other.y, z / other.z};
    }

    constexpr Vec3 &operator+=(const Vec3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    constexpr Vec3 &operator-=(const Vec3 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    constexpr Vec3 operator*(T scalar) const { return {x * scalar, y * scalar, z * scalar}; }
    constexpr Vec3 operator/(T scalar) const {
        debug::Assert(scalar != T{0}, "Division by zero");
        return {x / scalar, y / scalar, z / scalar};
    }

    constexpr bool operator==(const Vec3 &other) const { return x == other.x && y == other.y && z == other.z; }
    constexpr bool operator!=(const Vec3 &other) const { return x != other.x || y != other.y || z != other.z; }

    /**
     * @brief Calculate the squared length of the vector.
     * @return The squared length (x * x + y * y + z * z)
     * @note Faster than Length() as it avoids the square root operation
     */
    constexpr T LengthSquared() const { return x * x + y * y + z * z; }

    /**
     * @brief Calculate the length (magnitude) of the vector.
     * @return The Euclidean length sqrt(x * x + y * y + z * z)
     * @note Non-constexpr due to std::sqrt
     */
    T Length() const { return std::sqrt(LengthSquared()); }

    /**
     * @brief Normalize the vector in-place to unit length.
     * @return Reference to this vector after normalization
     * @note Asserts if the vector has zero length (use debug::Assert)
     */
    Vec3 &Normalize() {
        T len = Length();
        debug::Assert(len != T{0}, "Cannot normalize a vector with zero length");
        x /= len;
        y /= len;
        z /= len;
        return *this;
    }

    /**
     * @brief Get a normalized copy of this vector.
     * @return A new Vec3 normalized to unit length
     * @note Asserts if the vector has zero length (use debug::Assert)
     * @note This method is constexpr
     */
    constexpr Vec3 Normalized() const {
        T len = Length();
        debug::Assert(len != T{0}, "Cannot normalize a vector with zero length");
        return {x / len, y / len, z / len};
    }

    /**
     * @brief Calculate the dot product of two vectors.
     * @param a The first vector
     * @param b The second vector
     * @return The dot product (a.x * b.x + a.y * b.y + a.z * b.z)
     */
    static constexpr T Dot(const Vec3 &a, const Vec3 &b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

    /**
     * @brief Calculate the cross product of two vectors.
     *
     * The cross product produces a vector perpendicular to both input vectors,
     * with magnitude equal to the area of the parallelogram formed by them.
     *
     * @param a The first vector
     * @param b The second vector
     * @return The cross product vector perpendicular to both a and b
     * @note Order matters: Cross(a, b) = -Cross(b, a)
     * @note The result follows the right-hand rule
     */
    static constexpr Vec3 Cross(const Vec3 &a, const Vec3 &b) {
        return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    }
};

}  // namespace corelib::math

#ifdef __LOGENENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}  // namespace logenium

#endif  // __LOGENENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_MATH_VECTOR3_HXX
