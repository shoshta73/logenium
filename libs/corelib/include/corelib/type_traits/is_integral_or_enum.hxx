// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_TYPE_TRAITS_IS_INTEGRAL_OR_ENUM_HXX
#define LOGENIUM_CORELIB_TYPE_TRAITS_IS_INTEGRAL_OR_ENUM_HXX

#include <type_traits>

namespace corelib {

/**
 * @ingroup type_traits
 * @brief Type trait to check if a type is an integral type or enumeration.
 *
 * This trait checks whether a type (after removing references) is either an
 * integral type or an enumeration type. It filters out classes (to avoid
 * matching types with conversion operators), pointers, and floating-point types.
 *
 * The check is performed by:
 * 1. Removing reference qualifiers from T
 * 2. Ensuring the type is not a class (filters conversion operators)
 * 3. Ensuring the type is not a pointer
 * 4. Ensuring the type is not a floating-point type
 * 5. Checking if the type is an enum OR convertible to unsigned long long
 *
 * @tparam T The type to check
 *
 * ## Member Constants
 *
 * - `value`: true if T is an integral or enumeration type, false otherwise
 *
 * ## Helper Variable Template
 *
 * `is_integral_or_enum_v<T>` provides a convenient way to access the value:
 * @code
 * constexpr bool b = is_integral_or_enum_v<int>;  // true
 * @endcode
 *
 * @note References are stripped before checking
 * @note Classes with conversion operators to integral types are excluded
 *
 * Example:
 * @code
 * enum Color { Red, Green, Blue };
 * enum class Status { OK, Error };
 *
 * static_assert(is_integral_or_enum_v<int>);           // true
 * static_assert(is_integral_or_enum_v<unsigned long>); // true
 * static_assert(is_integral_or_enum_v<bool>);          // true
 * static_assert(is_integral_or_enum_v<char>);          // true
 * static_assert(is_integral_or_enum_v<Color>);         // true
 * static_assert(is_integral_or_enum_v<Status>);        // true
 * static_assert(is_integral_or_enum_v<int&>);          // true (reference stripped)
 *
 * static_assert(!is_integral_or_enum_v<float>);        // false (floating-point)
 * static_assert(!is_integral_or_enum_v<double>);       // false (floating-point)
 * static_assert(!is_integral_or_enum_v<int*>);         // false (pointer)
 * static_assert(!is_integral_or_enum_v<std::string>);  // false (class)
 * @endcode
 */
template <typename T>
class is_integral_or_enum {
    using UnderlyingT = std::remove_reference_t<T>;

  public:
    static const bool value = !std::is_class_v<UnderlyingT> &&  // Filter conversion operators.
                              !std::is_pointer_v<UnderlyingT> && !std::is_floating_point_v<UnderlyingT> &&
                              (std::is_enum_v<UnderlyingT> || std::is_convertible_v<UnderlyingT, unsigned long long>);
};

/**
 * @ingroup type_traits
 * @brief Helper variable template for is_integral_or_enum.
 *
 * Provides convenient access to is_integral_or_enum<T>::value following
 * the C++17 standard library convention.
 *
 * @tparam T The type to check
 *
 * Example:
 * @code
 * if constexpr (is_integral_or_enum_v<T>) {
 *     // T is integral or enum
 * }
 * @endcode
 */
template <typename T>
inline constexpr bool is_integral_or_enum_v = is_integral_or_enum<T>::value;

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_TYPE_TRAITS_IS_INTEGRAL_OR_ENUM_HXX
