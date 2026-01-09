// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_CASTING_ISA_HXX
#define LOGENIUM_CORELIB_CASTING_ISA_HXX

#include <corelib/detail/casting/cast.hxx>
#include <corelib/detail/casting/traits.hxx>
#include <corelib/internal/tracing.hxx>

namespace corelib {

/**
 * @ingroup casting
 * @brief Checks if a value is an instance of any of the specified types.
 *
 * isa() performs RTTI-free type checking using the classof() pattern. It checks
 * whether Val is an instance of any of the types specified in the template
 * parameter pack. Returns true if Val matches at least one type.
 *
 * This is the primary type-checking function in the casting system, supporting:
 * - Variadic type checking (check multiple types with OR semantics)
 * - References, pointers, const-qualified types
 * - std::unique_ptr (asserts non-null)
 * - LLVM-style classof() pattern (no C++ RTTI required)
 *
 * @tparam To The target type(s) to check for (variadic)
 * @tparam From The source type (deduced from Val)
 * @param Val The value to type-check
 * @return true if Val is an instance of any type in To..., false otherwise
 *
 * @note Requires To types to have static classof() methods
 * @note For pointers/unique_ptr, asserts Val is not null
 * @note Uses fold expression with || for variadic checking
 *
 * Example:
 * @code
 * Base *base = ...;
 *
 * // Single type check
 * if (isa<Derived>(base)) {
 *     // base points to a Derived instance
 * }
 *
 * // Multiple type check (OR semantics)
 * if (isa<Derived1, Derived2, Derived3>(base)) {
 *     // base is one of these types
 * }
 *
 * // Works with references
 * Base &ref = ...;
 * if (isa<Derived>(ref)) { }
 *
 * // Works with const
 * const Base *const_base = ...;
 * if (isa<Derived>(const_base)) { }
 * @endcode
 *
 * @see isa_and_present() for null-safe type checking
 * @see cast() for type casting with assertion
 * @see dyn_cast() for type casting with null on failure
 */
template <typename... To, typename From>
inline bool isa(const From &Val) {
    CRLB_ZONE_SCOPED;
    return (detail::CastInfo<To, const From>::IsPossible(Val) || ...);
}

/**
 * @ingroup casting
 * @brief Null-safe type checking with presence validation.
 *
 * isa_and_present() combines presence checking with type checking. It first
 * checks if Val is "present" (not null for pointers, has_value() for optional),
 * then performs isa<X...>() if the value is present.
 *
 * This is useful for nullable types where you want to check both presence
 * and type in a single operation, avoiding assertions from isa() on null values.
 *
 * @tparam X The target type(s) to check for (variadic)
 * @tparam Y The source type (deduced from Val)
 * @param Val The value to check
 * @return true if Val is present AND is an instance of any type in X...,
 *         false if Val is null/empty or not the right type
 *
 * @note Short-circuits: returns false immediately if Val is not present
 * @note Safe to call on null pointers or empty optionals (won't assert)
 * @note For nullable types: checks != nullptr for pointers, has_value() for optional
 *
 * Example:
 * @code
 * Base *base = ...;  // May be nullptr
 *
 * // Safe even if base is nullptr
 * if (isa_and_present<Derived>(base)) {
 *     // base is non-null AND points to Derived
 * }
 *
 * std::optional<Base> opt = ...;
 * if (isa_and_present<Derived>(opt)) {
 *     // opt has value AND is Derived
 * }
 *
 * // Variadic works too
 * if (isa_and_present<Derived1, Derived2>(base)) {
 *     // base is non-null AND is one of these types
 * }
 * @endcode
 *
 * @see isa() for type checking (asserts on null)
 * @see cast_if_present() for casting with presence check
 * @see dyn_cast_if_present() for dynamic casting with presence check
 */
template <typename... X, class Y>
inline bool isa_and_present(const Y &Val) {
    CRLB_ZONE_SCOPED;
    if (!detail::isPresent(Val)) return false;
    return isa<X...>(Val);
}

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_CASTING_ISA_HXX
