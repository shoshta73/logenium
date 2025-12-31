// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_TYPE_TRAITS_CONST_POINTER_OR_CONST_REF_HXX
#define LOGENIUM_CORELIB_TYPE_TRAITS_CONST_POINTER_OR_CONST_REF_HXX

#include <type_traits>

namespace corelib {

/**
 * @ingroup type_traits
 * @brief Type trait to create const pointer or const reference based on input type.
 *
 * This trait produces different const-qualified types depending on whether the
 * input is a pointer or not:
 * - For pointer types: creates a const pointer (pointer itself is const)
 * - For non-pointer types: creates a const lvalue reference
 *
 * Behavior:
 * - For pointer types: `T*` → `T* const` (const pointer to T)
 * - For non-pointer types: `T` → `const T&` (const reference to T)
 *
 * This is useful for creating const-correct parameter types where pointers should
 * be const pointers and non-pointers should be const references.
 *
 * @tparam T The type to transform into const pointer or const reference
 *
 * ## Member Types
 *
 * - `type`: The resulting const pointer or const reference type
 *
 * ## Helper Type Alias
 *
 * `const_pointer_or_const_ref_t<T>` provides a convenient way to access the type:
 * @code
 * using T = const_pointer_or_const_ref_t<int>;  // const int&
 * @endcode
 *
 * @note This trait is primarily used internally by the casting module for creating
 *       const-correct types in return type calculations
 * @note For pointer types, the pointer itself becomes const, not the pointee
 *
 * Example:
 * @code
 * // Non-pointer types - creates const reference
 * static_assert(std::is_same_v<const_pointer_or_const_ref_t<int>, const int&>);
 * static_assert(std::is_same_v<const_pointer_or_const_ref_t<double>, const double&>);
 * static_assert(std::is_same_v<const_pointer_or_const_ref_t<std::string>, const std::string&>);
 *
 * // Pointer types - creates const pointer (pointer itself is const)
 * static_assert(std::is_same_v<const_pointer_or_const_ref_t<int*>, int* const>);
 * static_assert(std::is_same_v<const_pointer_or_const_ref_t<char*>, char* const>);
 * static_assert(std::is_same_v<const_pointer_or_const_ref_t<const int*>, const int* const>);
 *
 * // Note: For pointers, the pointer is const, not the pointee
 * int* const p1 = ...;  // const pointer to int (can't change pointer)
 * const int* p2 = ...;  // pointer to const int (can't change pointee)
 *
 * // Already const types
 * static_assert(std::is_same_v<const_pointer_or_const_ref_t<const int>, const int&>);
 *
 * // Use case: const-correct function parameters
 * template <typename T>
 * void process(const_pointer_or_const_ref_t<T> param) {
 *     // param is const int& for int, int* const for int*, etc.
 * }
 * @endcode
 */
template <typename T>
struct const_pointer_or_const_ref {
    using type = std::conditional_t<std::is_pointer_v<T>, const T, const T &>;
};

/**
 * @ingroup type_traits
 * @brief Helper type alias for const_pointer_or_const_ref.
 *
 * Provides convenient access to const_pointer_or_const_ref<T>::type following
 * the C++14 standard library convention.
 *
 * @tparam T The type to transform into const pointer or const reference
 *
 * Example:
 * @code
 * template <typename T>
 * void safe_read(const_pointer_or_const_ref_t<T> value) {
 *     // value is a const reference for non-pointers, const pointer for pointers
 *     // Ensures the function can't modify the original value
 * }
 * @endcode
 */
template <typename T>
using const_pointer_or_const_ref_t = typename const_pointer_or_const_ref<T>::type;

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_TYPE_TRAITS_CONST_POINTER_OR_CONST_REF_HXX
