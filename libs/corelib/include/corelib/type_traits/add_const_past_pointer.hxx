// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_TYPE_TRAITS_ADD_CONST_PAST_POINTER_HXX
#define LOGENIUM_CORELIB_TYPE_TRAITS_ADD_CONST_PAST_POINTER_HXX

#include <type_traits>

namespace corelib {

/**
 * @ingroup type_traits
 * @brief Type trait to add const qualification past pointer indirection.
 *
 * This trait adds const qualification to a type, but if the type is a pointer,
 * it adds const to the pointee (the type being pointed to) rather than to the
 * pointer itself.
 *
 * Behavior:
 * - For pointer types: `T*` → `const T*` (const pointee)
 * - For non-pointer types: `T` → `const T` (const type)
 *
 * This is different from `std::add_const` which would produce:
 * - `T*` → `T* const` (const pointer, non-const pointee)
 *
 * @tparam T The type to add const to
 *
 * ## Member Types
 *
 * - `type`: The resulting type with const added past pointer indirection
 *
 * ## Helper Type Alias
 *
 * `add_const_past_pointer_t<T>` provides a convenient way to access the type:
 * @code
 * using T = add_const_past_pointer_t<int*>;  // const int*
 * @endcode
 *
 * @note This trait is primarily used internally by the casting module to preserve
 *       const-correctness when working with pointer types
 *
 * Example:
 * @code
 * // Pointer types - adds const to pointee
 * static_assert(std::is_same_v<add_const_past_pointer_t<int*>, const int*>);
 * static_assert(std::is_same_v<add_const_past_pointer_t<char*>, const char*>);
 * static_assert(std::is_same_v<add_const_past_pointer_t<void*>, const void*>);
 *
 * // Already const pointee - no change
 * static_assert(std::is_same_v<add_const_past_pointer_t<const int*>, const int*>);
 *
 * // Non-pointer types - adds const to type itself
 * static_assert(std::is_same_v<add_const_past_pointer_t<int>, const int>);
 * static_assert(std::is_same_v<add_const_past_pointer_t<double>, const double>);
 *
 * // Compare with std::add_const (different behavior for pointers)
 * static_assert(std::is_same_v<std::add_const_t<int*>, int* const>);  // const pointer
 * static_assert(std::is_same_v<add_const_past_pointer_t<int*>, const int*>);  // const pointee
 * @endcode
 */
template <typename T>
struct add_const_past_pointer {
    using type = std::conditional_t<std::is_pointer_v<T>, const std::remove_pointer_t<T> *, const T>;
};

/**
 * @ingroup type_traits
 * @brief Helper type alias for add_const_past_pointer.
 *
 * Provides convenient access to add_const_past_pointer<T>::type following
 * the C++14 standard library convention.
 *
 * @tparam T The type to add const to
 *
 * Example:
 * @code
 * template <typename T>
 * void foo(add_const_past_pointer_t<T> param) {
 *     // param has const added past pointer indirection
 * }
 * @endcode
 */
template <typename T>
using add_const_past_pointer_t = typename add_const_past_pointer<T>::type;

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_TYPE_TRAITS_ADD_CONST_PAST_POINTER_HXX
