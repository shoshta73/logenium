// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_TYPE_TRAITS_HXX
#define LOGENIUM_CORELIB_TYPE_TRAITS_HXX

/**
 * @file type_traits.hxx
 * @brief Convenience header for all type traits module components
 *
 * This header includes all custom type traits used throughout corelib. These
 * traits extend the standard library's type traits with additional utilities
 * for const propagation, reference handling, and type categorization.
 */

/**
 * @defgroup type_traits Type Traits Module
 * @brief Custom type traits extending std::type_traits
 *
 * The type traits module provides compile-time type manipulation utilities that
 * extend the C++ standard library's type traits. These are primarily used internally
 * by the casting module but are exposed for general use.
 *
 * ## Components
 *
 * ### Const Qualification
 * - `add_const_past_pointer<T>`: Add const past pointer indirection
 * - `const_pointer_or_const_ref<T>`: Conditional const pointer/reference selection
 *
 * ### Reference Handling
 * - `add_lvalue_reference_if_not_pointer<T>`: Conditional lvalue reference addition
 *
 * ### Type Classification
 * - `is_integral_or_enum<T>`: Check if type is integral or enumeration
 *
 * ## Usage
 *
 * These traits follow the standard library conventions with `_t` suffixes for
 * type aliases and `_v` suffixes for value traits:
 *
 * @code
 * // Type transformation
 * using T1 = add_const_past_pointer_t<int*>;  // int* const (const pointer)
 * using T2 = const_pointer_or_const_ref_t<int*>;  // const int*
 *
 * // Type checking
 * constexpr bool b = is_integral_or_enum_v<MyEnum>;  // true
 * @endcode
 *
 * Include the convenience header:
 * @code
 * #include <corelib/type_traits.hxx>
 * @endcode
 *
 * Or include specific headers:
 * @code
 * #include <corelib/type_traits/add_const_past_pointer.hxx>
 * #include <corelib/type_traits/is_integral_or_enum.hxx>
 * @endcode
 */

#include <type_traits>  // IWYU pragma: export

#include <corelib/type_traits/add_const_past_pointer.hxx>               // IWYU pragma: export
#include <corelib/type_traits/add_lvalue_reference_if_not_pointer.hxx>  // IWYU pragma: export
#include <corelib/type_traits/const_pointer_or_const_ref.hxx>           // IWYU pragma: export
#include <corelib/type_traits/is_integral_or_enum.hxx>                  // IWYU pragma: export

#endif  // LOGENIUM_CORELIB_TYPE_TRAITS_HXX
