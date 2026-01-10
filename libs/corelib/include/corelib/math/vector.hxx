// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_MATH_VECTOR_HXX
#define LOGENIUM_CORELIB_MATH_VECTOR_HXX

/**
 * @file vector.hxx
 * @ingroup math
 * @brief Convenience header for all vector classes
 *
 * This header includes all vector class implementations (Vec1, Vec2, Vec3, Vec4)
 * and their common type aliases. Include this file to get access to all vector
 * classes at once.
 */

/**
 * @ingroup math
 * @defgroup math_vectors Vector Classes
 * @brief Template-based vector classes for mathematical operations.
 *
 * This module provides vector classes for 1D, 2D, 3D, and 4D mathematical operations.
 * All vector classes are template-based and support various numeric types (float, double, int, etc.).
 *
 * ## Features
 *
 * - **Template-based**: Works with any numeric type
 * - **Constexpr support**: Most operations can be evaluated at compile time
 * - **Multiple naming schemes**: Support for different conventions
 *   - Spatial: x, y, z, w
 *   - Color: r, g, b, a
 *   - Texture: s, t, p, q
 * - **Rich operation set**: Arithmetic, scalar operations, dot/cross products
 * - **Safety**: Debug assertions for division by zero and zero-length normalization
 *
 * ## Vector Classes
 *
 * - `Vec1<T>`: 1D vector (x/r/s)
 * - `Vec2<T>`: 2D vector (x/y, r/g, s/t)
 * - `Vec3<T>`: 3D vector (x/y/z, r/g/b, s/t/p) with Cross product
 * - `Vec4<T>`: 4D vector (x/y/z/w, r/g/b/a, s/t/p/q)
 *
 * ## Common Type Aliases
 *
 * Each vector class provides standard type aliases:
 * - `dvec1`, `dvec2`, `dvec3`, `dvec4`: Double-precision (double)
 * - `ivec1`, `ivec2`, `ivec3`, `ivec4`: Signed integer (int)
 * - `uvec1`, `uvec2`, `uvec3`, `uvec4`: Unsigned integer (unsigned int)
 *
 * ## Common Operations
 *
 * All vector classes support:
 * - **Constructors**: Default (zero), uniform value, component-wise
 * - **Static factories**: Zero(), One()
 * - **Arithmetic**: operator+, operator-, operator*, operator/ (component-wise and scalar)
 * - **Compound assignment**: operator+=, operator-=
 * - **Comparison**: operator==, operator!=
 * - **Length**: LengthSquared(), Length()
 * - **Normalization**: Normalize() (mutating), Normalized() (const)
 * - **Dot product**: Static Dot(a, b) method
 * - **Cross product**: Static Cross(a, b) method (Vec3 only)
 *
 * ## Usage Examples
 *
 * @code
 * using namespace corelib::math;
 *
 * // Construction
 * Vec3<float> zero;                      // (0, 0, 0)
 * Vec3<float> ones{1.0f};                // (1, 1, 1)
 * Vec3<float> pos{1.0f, 2.0f, 3.0f};     // (1, 2, 3)
 *
 * // Arithmetic operations
 * Vec3<float> a{1.0f, 2.0f, 3.0f};
 * Vec3<float> b{4.0f, 5.0f, 6.0f};
 * auto sum = a + b;                      // (5, 7, 9)
 * auto scaled = a * 2.0f;                // (2, 4, 6)
 *
 * // Dot and cross products
 * float dot = Vec3<float>::Dot(a, b);
 * Vec3<float> cross = Vec3<float>::Cross(a, b);
 *
 * // Normalization
 * Vec3<float> dir{3.0f, 0.0f, 4.0f};
 * float len = dir.Length();              // 5.0
 * auto unit = dir.Normalized();          // (0.6, 0, 0.8)
 * dir.Normalize();                       // Mutates dir to (0.6, 0, 0.8)
 *
 * // Color naming scheme
 * Vec4<float> color{1.0f, 0.5f, 0.25f, 1.0f};
 * color.r = 0.8f;  // Modify red component
 * color.a = 0.5f;  // Modify alpha component
 *
 * // Type aliases
 * dvec3 position{1.0, 2.0, 3.0};         // double precision
 * ivec2 screenPos{800, 600};             // integer coordinates
 * uvec4 rgba{255, 128, 64, 255};         // unsigned color
 * @endcode
 *
 * @see Vec1
 * @see Vec2
 * @see Vec3
 * @see Vec4
 */

#include <corelib/math/vector1.hxx>  // IWYU pragma: export
#include <corelib/math/vector2.hxx>  // IWYU pragma: export
#include <corelib/math/vector3.hxx>  // IWYU pragma: export
#include <corelib/math/vector4.hxx>  // IWYU pragma: export

#endif  // LOGENIUM_CORELIB_MATH_VECTOR_HXX
