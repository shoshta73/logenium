// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_TYPE_TRAITS_ADD_LVALUE_REFERENCE_IF_NOT_POINTER_HXX
#define LOGENIUM_CORELIB_TYPE_TRAITS_ADD_LVALUE_REFERENCE_IF_NOT_POINTER_HXX

#include <type_traits>

namespace corelib {

/**
 * @ingroup type_traits
 * @brief Type trait to conditionally add lvalue reference to non-pointer types.
 *
 * This trait adds an lvalue reference to a type only if it is not a pointer type.
 * Pointer types are returned unchanged.
 *
 * Behavior:
 * - For pointer types: `T*` → `T*` (unchanged)
 * - For non-pointer types: `T` → `T&` (lvalue reference added)
 *
 * This is different from `std::add_lvalue_reference` which always adds a reference:
 * - `std::add_lvalue_reference<T*>` → `T*&` (reference to pointer)
 * - `add_lvalue_reference_if_not_pointer<T*>` → `T*` (unchanged)
 *
 * @tparam T The type to conditionally add lvalue reference to
 *
 * ## Member Types
 *
 * - `type`: The resulting type (T& for non-pointers, T for pointers)
 *
 * ## Helper Type Alias
 *
 * `add_lvalue_reference_if_not_pointer_t<T>` provides a convenient way to access the type:
 * @code
 * using T = add_lvalue_reference_if_not_pointer_t<int>;  // int&
 * @endcode
 *
 * @note This trait is primarily used internally by the casting module for return
 *       type calculations, ensuring pointers remain pointers and non-pointers
 *       become references
 *
 * Example:
 * @code
 * // Non-pointer types - adds lvalue reference
 * static_assert(std::is_same_v<add_lvalue_reference_if_not_pointer_t<int>, int&>);
 * static_assert(std::is_same_v<add_lvalue_reference_if_not_pointer_t<double>, double&>);
 * static_assert(std::is_same_v<add_lvalue_reference_if_not_pointer_t<std::string>, std::string&>);
 *
 * // Already a reference - still adds reference (T& → T& via reference collapsing)
 * static_assert(std::is_same_v<add_lvalue_reference_if_not_pointer_t<int&>, int&>);
 *
 * // Pointer types - unchanged
 * static_assert(std::is_same_v<add_lvalue_reference_if_not_pointer_t<int*>, int*>);
 * static_assert(std::is_same_v<add_lvalue_reference_if_not_pointer_t<const char*>, const char*>);
 * static_assert(std::is_same_v<add_lvalue_reference_if_not_pointer_t<void*>, void*>);
 *
 * // Compare with std::add_lvalue_reference (different behavior for pointers)
 * static_assert(std::is_same_v<std::add_lvalue_reference_t<int*>, int*&>);  // reference to pointer
 * static_assert(std::is_same_v<add_lvalue_reference_if_not_pointer_t<int*>, int*>);  // pointer unchanged
 * @endcode
 */
template <typename T>
struct add_lvalue_reference_if_not_pointer {
    using type = std::conditional_t<std::is_pointer_v<T>, T, T &>;
};

/**
 * @ingroup type_traits
 * @brief Helper type alias for add_lvalue_reference_if_not_pointer.
 *
 * Provides convenient access to add_lvalue_reference_if_not_pointer<T>::type
 * following the C++14 standard library convention.
 *
 * @tparam T The type to conditionally add lvalue reference to
 *
 * Example:
 * @code
 * template <typename T>
 * add_lvalue_reference_if_not_pointer_t<T> get_value(T val) {
 *     // Returns T& for non-pointers, T for pointers
 *     return val;
 * }
 * @endcode
 */
template <typename T>
using add_lvalue_reference_if_not_pointer_t = typename add_lvalue_reference_if_not_pointer<T>::type;

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_TYPE_TRAITS_ADD_LVALUE_REFERENCE_IF_NOT_POINTER_HXX
