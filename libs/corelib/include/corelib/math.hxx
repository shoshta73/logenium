// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_MATH_HXX
#define LOGENIUM_CORELIB_MATH_HXX

/**
 * @file math.hxx
 * @brief Convenience header for all math module components
 *
 * This header includes all mathematical utilities: vector classes (Vec1, Vec2, Vec3, Vec4)
 * and common type aliases. Include this file to get access to all math features at once.
 */

/**
 * @defgroup math Mathematical Utilities
 * @ingroup corelib
 * @brief Mathematical utilities including vector operations and common math functions.
 *
 * The math module provides template-based vector classes for 1D, 2D, 3D, and 4D
 * mathematical operations. These vectors support various numeric types and provide
 * common mathematical operations like addition, subtraction, dot products, cross products,
 * normalization, and length calculations.
 *
 * ## Features
 *
 * - **Template-based**: Works with any numeric type (float, double, int, etc.)
 * - **Constexpr support**: Most operations can be evaluated at compile time
 * - **Multiple naming schemes**: x/y/z/w, r/g/b/a, s/t/p/q for different use cases
 * - **Type-safe**: Compile-time type checking with debug assertions
 * - **Zero-cost abstractions**: Optimized for performance with inlining
 *
 * ## Components
 *
 * ### Vector Classes
 * - `Vec1<T>`: 1D vector with x/r/s accessors
 * - `Vec2<T>`: 2D vector with x/y, r/g, s/t accessors
 * - `Vec3<T>`: 3D vector with x/y/z, r/g/b, s/t/p accessors (includes Cross product)
 * - `Vec4<T>`: 4D vector with x/y/z/w, r/g/b/a, s/t/p/q accessors
 *
 * ### Common Type Aliases
 * - `dvec1`, `dvec2`, `dvec3`, `dvec4`: Double-precision vectors
 * - `ivec1`, `ivec2`, `ivec3`, `ivec4`: Integer vectors
 * - `uvec1`, `uvec2`, `uvec3`, `uvec4`: Unsigned integer vectors
 *
 * ## Usage
 *
 * Include the convenience header to get all utilities:
 * @code
 * #include <corelib/math.hxx>
 *
 * using namespace corelib::math;
 *
 * // 3D vector operations
 * Vec3<float> a{1.0f, 2.0f, 3.0f};
 * Vec3<float> b{4.0f, 5.0f, 6.0f};
 *
 * auto sum = a + b;
 * auto dot = Vec3<float>::Dot(a, b);
 * auto cross = Vec3<float>::Cross(a, b);
 * auto normalized = a.Normalized();
 *
 * // Type aliases
 * dvec3 position{1.0, 2.0, 3.0};  // double precision
 * ivec2 screenPos{100, 200};      // integer coordinates
 * @endcode
 *
 * Or include specific headers:
 * @code
 * #include <corelib/math/vector1.hxx>
 * #include <corelib/math/vector2.hxx>
 * #include <corelib/math/vector3.hxx>
 * #include <corelib/math/vector4.hxx>
 * @endcode
 */

#include <corelib/math/vector.hxx>  // IWYU pragma: export

#endif  // LOGENIUM_CORELIB_MATH_HXX
