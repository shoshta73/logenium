// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_DETAIL_CASTING_TRAITS_HXX
#define LOGENIUM_CORELIB_DETAIL_CASTING_TRAITS_HXX

#include <cstddef>
#include <optional>
#include <type_traits>

#include <corelib/type_traits/add_const_past_pointer.hxx>
#include <corelib/type_traits/add_lvalue_reference_if_not_pointer.hxx>

namespace corelib::detail {

// =============================================================================
// Section 1: Type Simplification
// =============================================================================

/**
 * @ingroup casting_detail
 * @brief Base template for recursive type simplification in the casting system.
 *
 * SimplifyType provides a mechanism to recursively unwrap type qualifiers
 * (particularly const) to find the underlying "simple" type. This is a core
 * component of the casting infrastructure, enabling const-correct casts through
 * layers of type qualifiers.
 *
 * The base template represents the identity case - a type that is already
 * "simple" (has no additional qualifiers to strip). The specialization for
 * const types (SimplifyType<const From>) performs the actual recursive unwrapping.
 *
 * @tparam From The type to simplify
 *
 * ## Member Types
 *
 * - `SimpleType`: The simplified version of From (in base template, From itself)
 *
 * ## Static Methods
 *
 * - `GetSimplifiedValue(From &Val)`: Returns a reference to the simplified value
 *
 * @todo Rename this and/or replace it with other cast traits
 *
 * @note This is an internal detail type trait used by the casting infrastructure
 * @note The const specialization performs recursive unwrapping through type layers
 *
 * Example:
 * @code
 * // Base case - type is already simple
 * static_assert(std::is_same_v<SimplifyType<int>::SimpleType, int>);
 * static_assert(std::is_same_v<SimplifyType<int*>::SimpleType, int*>);
 *
 * // Const unwrapping happens in the specialization
 * // SimplifyType<const int>::SimpleType → const int (via const specialization)
 * // SimplifyType<const int*>::SimpleType → const int* (pointer to const)
 * @endcode
 *
 * @see SimplifyType<const From> for const unwrapping behavior
 * @see IsSimpleType for checking if a type is already simplified
 */
template <typename From>
struct SimplifyType {
    using SimpleType = From;

    static SimpleType &GetSimplifiedValue(From &Val) { return Val; }
};

/**
 * @ingroup casting_detail
 * @brief Specialization for const-qualified types that recursively unwraps const.
 *
 * This specialization handles const-qualified types by recursively applying
 * SimplifyType to the non-const type, then re-applying const correctly based
 * on whether the simplified type is a pointer or not.
 *
 * The unwrapping process:
 * 1. Strip const from From to get the underlying type
 * 2. Recursively simplify the underlying type
 * 3. Re-apply const past pointer indirection (for pointers, constifies pointee)
 * 4. Add lvalue reference if not a pointer
 *
 * @tparam From The const-qualified type to simplify
 *
 * ## Member Types
 *
 * - `NonConstSimpleType`: The simplified type without const
 * - `SimpleType`: NonConstSimpleType with const re-applied past pointers
 * - `RetType`: SimpleType with lvalue reference added (if not a pointer)
 *
 * ## Static Methods
 *
 * - `GetSimplifiedValue(const From &Val)`: Returns a reference to the simplified
 *   value, preserving const-correctness by recursively delegating to the
 *   non-const SimplifyType and casting away const safely
 *
 * @note Uses add_const_past_pointer_t to preserve const-correctness for pointers
 * @note Uses add_lvalue_reference_if_not_pointer_t for non-pointer return types
 *
 * Example:
 * @code
 * // Const on non-pointer types
 * static_assert(std::is_same_v<SimplifyType<const int>::SimpleType, const int>);
 *
 * // Const on pointer types - const applies to pointee
 * static_assert(std::is_same_v<SimplifyType<const int*>::SimpleType, const int*>);
 *
 * // Multiple levels of const unwrapping
 * // SimplifyType<const int*> → SimpleType = const int*
 * @endcode
 *
 * @see add_const_past_pointer_t for const propagation behavior
 * @see add_lvalue_reference_if_not_pointer_t for reference addition
 */
template <typename From>
struct SimplifyType<const From> {
    using NonConstSimpleType = typename SimplifyType<From>::SimpleType;
    using SimpleType = add_const_past_pointer_t<NonConstSimpleType>;
    using RetType = add_lvalue_reference_if_not_pointer_t<SimpleType>;

    static RetType GetSimplifiedValue(const From &Val) {
        return SimplifyType<From>::GetSimplifiedValue(const_cast<From &>(Val));
    }
};

/**
 * @ingroup casting_detail
 * @brief Type trait to check if a type is already in simplified form.
 *
 * IsSimpleType checks whether a type X is equal to its SimplifyType<X>::SimpleType.
 * A type is considered "simple" when SimplifyType produces the same type,
 * indicating no further simplification is possible.
 *
 * This is useful for optimizations and compile-time checks in the casting
 * infrastructure to determine if type simplification is needed.
 *
 * @tparam X The type to check
 *
 * ## Member Constants
 *
 * - `value`: true if X is already simplified (X == SimplifyType<X>::SimpleType),
 *   false otherwise
 *
 * @note This is an internal trait used by the casting system
 *
 * Example:
 * @code
 * // Already simple types
 * static_assert(IsSimpleType<int>::value);
 * static_assert(IsSimpleType<int*>::value);
 * static_assert(IsSimpleType<const int*>::value);
 *
 * // Types requiring simplification would be detected here
 * // (behavior depends on SimplifyType specializations)
 * @endcode
 *
 * @see SimplifyType for the simplification mechanism
 */
template <class X>
struct IsSimpleType {
    static const bool value = std::is_same_v<X, typename SimplifyType<X>::SimpleType>;
};

// =============================================================================
// Section 2: Nullable Type Detection
// =============================================================================

/**
 * @ingroup casting_detail
 * @brief Variable template to detect if a type can represent a null/empty state.
 *
 * IsNullable checks whether a type T can be constructed from or compared with
 * nullptr, indicating it has nullable semantics. This includes:
 * - All pointer types (T*)
 * - Types constructible from std::nullptr_t (like std::unique_ptr, smart pointers)
 *
 * This trait is used by the casting system to determine if a type supports
 * null-safe operations like dyn_cast_or_null() and cast_or_null().
 *
 * @tparam T The type to check for nullable semantics
 *
 * @note Returns true for raw pointers and smart pointers
 * @note Returns false for value types and references
 * @note Used internally by ValueIsPresent to specialize for nullable types
 *
 * Example:
 * @code
 * // Pointer types - nullable
 * static_assert(IsNullable<int*>);
 * static_assert(IsNullable<const char*>);
 * static_assert(IsNullable<void*>);
 *
 * // Smart pointers - nullable (constructible from nullptr)
 * static_assert(IsNullable<std::unique_ptr<int>>);
 *
 * // Value types - not nullable
 * static_assert(!IsNullable<int>);
 * static_assert(!IsNullable<double>);
 * static_assert(!IsNullable<std::string>);
 *
 * // std::optional is not considered "nullable" in this sense
 * // (it's not constructible from nullptr, uses std::nullopt instead)
 * static_assert(!IsNullable<std::optional<int>>);
 * @endcode
 *
 * @see ValueIsPresent for how nullable types are handled in presence checking
 */
template <typename T>
constexpr bool IsNullable = std::is_pointer_v<T> || std::is_constructible_v<T, std::nullptr_t>;

// =============================================================================
// Section 3: Value Presence Detection
// =============================================================================

/**
 * @ingroup casting_detail
 * @brief Base template for detecting value presence and unwrapping values.
 *
 * ValueIsPresent provides a uniform interface for checking if a value is
 * "present" (not null, not empty) and for unwrapping container types to
 * access the underlying value. This is a core component enabling null-safe
 * casting operations.
 *
 * The base template handles regular (non-nullable, non-optional) types,
 * where values are always present. Specializations handle std::optional
 * and nullable types (pointers, types constructible from nullptr).
 *
 * @tparam T The type to check for presence
 * @tparam Enable SFINAE enabler for specializations
 *
 * ## Member Types
 *
 * - `UnwrappedType`: The underlying value type (T for base template)
 *
 * ## Static Methods
 *
 * - `IsPresent(const T &t)`: Always returns true for regular types
 * - `UnwrapValue(T &t)`: Returns the value unchanged (no unwrapping needed)
 *
 * @note Specializations exist for std::optional and nullable types
 * @note Used by cast_if_present(), dyn_cast_if_present(), etc.
 *
 * Example:
 * @code
 * // Regular types - always present
 * int x = 42;
 * static_assert(ValueIsPresent<int>::IsPresent(x));
 * static_assert(ValueIsPresent<int>::UnwrapValue(x) == 42);
 *
 * std::string s = "hello";
 * static_assert(ValueIsPresent<std::string>::IsPresent(s));
 * @endcode
 *
 * @see ValueIsPresent<std::optional<T>> for optional value handling
 * @see ValueIsPresent<T, enable_if nullable> for nullable type handling
 */
template <typename T, typename Enable = void>
struct ValueIsPresent {
    using UnwrappedType = T;
    static inline bool IsPresent(const T &t) { return true; }
    static inline decltype(auto) UnwrapValue(T &t) { return t; }
};

/**
 * @ingroup casting_detail
 * @brief Specialization for std::optional<T> that checks has_value() and unwraps.
 *
 * This specialization handles std::optional types by:
 * - Checking presence via has_value()
 * - Unwrapping via operator* to access the contained value
 *
 * @tparam T The type contained in the optional
 *
 * ## Member Types
 *
 * - `UnwrappedType`: The contained type T (not std::optional<T>)
 *
 * ## Static Methods
 *
 * - `IsPresent(const std::optional<T> &t)`: Returns t.has_value()
 * - `UnwrapValue(std::optional<T> &t)`: Returns *t (the contained value)
 *
 * @note Enables cast_if_present() and dyn_cast_if_present() to work with optionals
 * @note UnwrapValue assumes the optional contains a value (call IsPresent first)
 *
 * Example:
 * @code
 * std::optional<int> opt1 = 42;
 * std::optional<int> opt2 = std::nullopt;
 *
 * static_assert(ValueIsPresent<std::optional<int>>::IsPresent(opt1));  // true
 * static_assert(!ValueIsPresent<std::optional<int>>::IsPresent(opt2)); // false
 *
 * auto &value = ValueIsPresent<std::optional<int>>::UnwrapValue(opt1);
 * // value is int& = 42
 * @endcode
 */
template <typename T>
struct ValueIsPresent<std::optional<T>> {
    using UnwrappedType = T;
    static inline bool IsPresent(const std::optional<T> &t) { return t.has_value(); }
    static inline decltype(auto) UnwrapValue(std::optional<T> &t) { return *t; }
};

/**
 * @ingroup casting_detail
 * @brief Specialization for nullable types (pointers and nullptr-constructible types).
 *
 * This specialization handles types that can be null-checked against nullptr:
 * - Raw pointers (int*, char*, etc.)
 * - Smart pointers (std::unique_ptr, types constructible from nullptr)
 *
 * Presence is checked by comparing against T(nullptr).
 *
 * @tparam T The nullable type (must satisfy IsNullable<T>)
 *
 * ## Member Types
 *
 * - `UnwrappedType`: The nullable type T itself (no unwrapping for pointers)
 *
 * ## Static Methods
 *
 * - `IsPresent(const T &t)`: Returns true if t != T(nullptr)
 * - `UnwrapValue(T &t)`: Returns the value unchanged (pointers don't need unwrapping)
 *
 * @note Enabled via SFINAE when IsNullable<T> is true
 * @note For pointers, UnwrappedType is still the pointer type (dereference happens elsewhere)
 * @note Enables cast_or_null() and dyn_cast_or_null() to work with pointers
 *
 * Example:
 * @code
 * int x = 42;
 * int *ptr1 = &x;
 * int *ptr2 = nullptr;
 *
 * static_assert(ValueIsPresent<int*>::IsPresent(ptr1));  // true
 * static_assert(!ValueIsPresent<int*>::IsPresent(ptr2)); // false
 *
 * auto unwrapped = ValueIsPresent<int*>::UnwrapValue(ptr1);
 * // unwrapped is int* (same as ptr1, no dereferencing)
 *
 * std::unique_ptr<int> uptr = std::make_unique<int>(42);
 * static_assert(ValueIsPresent<std::unique_ptr<int>>::IsPresent(uptr));
 * @endcode
 *
 * @see IsNullable for the nullable type detection trait
 */
template <typename T>
struct ValueIsPresent<T, std::enable_if_t<detail::IsNullable<T>>> {
    using UnwrappedType = T;
    static inline bool IsPresent(const T &t) { return t != T(nullptr); }
    static inline decltype(auto) UnwrapValue(T &t) { return t; }
};

/**
 * @ingroup casting_detail
 * @brief Helper function to check if a value is present (not null/empty).
 *
 * This function provides a convenient interface to ValueIsPresent::IsPresent()
 * that also handles type simplification. It first simplifies the type T
 * (unwrapping const qualifiers), then checks presence on the simplified value.
 *
 * Works with:
 * - Regular types (always present)
 * - std::optional (checks has_value())
 * - Pointers and smart pointers (checks != nullptr)
 *
 * @tparam T The type to check (const qualifiers are automatically handled)
 * @param t The value to check for presence
 * @return true if the value is present (not null/empty), false otherwise
 *
 * @note Automatically handles const-qualified types via SimplifyType
 * @note Used internally by cast_if_present() and dyn_cast_if_present()
 *
 * Example:
 * @code
 * int x = 42;
 * static_assert(isPresent(x));  // true - regular values always present
 *
 * int *ptr = &x;
 * static_assert(isPresent(ptr));  // true - non-null pointer
 *
 * int *null_ptr = nullptr;
 * static_assert(!isPresent(null_ptr));  // false - null pointer
 *
 * std::optional<int> opt = 42;
 * static_assert(isPresent(opt));  // true - optional has value
 *
 * std::optional<int> empty;
 * static_assert(!isPresent(empty));  // false - optional is empty
 * @endcode
 *
 * @see ValueIsPresent for the underlying presence checking mechanism
 * @see unwrapValue() for extracting the value after checking presence
 */
template <typename T>
inline bool isPresent(const T &t) {
    return ValueIsPresent<typename SimplifyType<T>::SimpleType>::IsPresent(
        SimplifyType<T>::GetSimplifiedValue(const_cast<T &>(t)));
}

/**
 * @ingroup casting_detail
 * @brief Helper function to unwrap a value from its container.
 *
 * This function provides a convenient interface to ValueIsPresent::UnwrapValue().
 * For most types, this is an identity operation (returns the value unchanged).
 * For std::optional, it dereferences to get the contained value.
 *
 * @tparam T The type to unwrap
 * @param t The value to unwrap
 * @return The unwrapped value (decltype(auto) preserves reference type)
 *
 * @note For std::optional, assumes the value is present (call isPresent() first)
 * @note For pointers, returns the pointer unchanged (no dereferencing)
 * @note Uses decltype(auto) to preserve value category and reference type
 *
 * Example:
 * @code
 * std::optional<int> opt = 42;
 * auto &value = unwrapValue(opt);
 * // value is int& = 42
 *
 * int x = 10;
 * auto &same = unwrapValue(x);
 * // same is int& = 10 (identity operation)
 *
 * int *ptr = &x;
 * auto p = unwrapValue(ptr);
 * // p is int* (pointer unchanged, not dereferenced)
 * @endcode
 *
 * @see ValueIsPresent for the underlying unwrapping mechanism
 * @see isPresent() for checking if a value should be unwrapped
 */
template <typename T>
inline decltype(auto) unwrapValue(T &t) {
    return ValueIsPresent<T>::UnwrapValue(t);
}

// =============================================================================
// Section 4: Utilities
// =============================================================================

/**
 * @ingroup casting_detail
 * @brief Helper type alias for CRTP pattern with optional derived class override.
 *
 * SelfType implements a common CRTP (Curiously Recurring Template Pattern)
 * idiom where a base class template takes an optional derived type parameter.
 * If the derived type is void, it defaults to a specified default type.
 *
 * This is used extensively in the casting infrastructure to allow customization
 * points while providing sensible defaults. Template classes can use SelfType
 * to refer to either a user-provided derived class or themselves.
 *
 * @tparam OptionalDerived The optional derived class (or void for default)
 * @tparam Default The default type to use when OptionalDerived is void
 *
 * Pattern:
 * @code
 * // Base class using SelfType for CRTP
 * template <typename To, typename From, typename Derived = void>
 * struct CastOperation {
 *     using Self = SelfType<Derived, CastOperation<To, From>>;
 *
 *     // Methods can call static_cast<Self*>(this) to access derived overrides
 * };
 *
 * // User can provide custom derived class
 * struct MyCustomCast : CastOperation<int, double, MyCustomCast> {
 *     // Override behavior here
 * };
 *
 * // Or use the default (Derived = void)
 * using DefaultCast = CastOperation<int, double>;
 * @endcode
 *
 * @note Used by casting infrastructure classes like DefaultDoCastIfPossible
 * @note Enables customization without requiring explicit derived classes
 *
 * Example:
 * @code
 * // When OptionalDerived is a type
 * static_assert(std::is_same_v<SelfType<MyClass, DefaultClass>, MyClass>);
 *
 * // When OptionalDerived is void
 * static_assert(std::is_same_v<SelfType<void, DefaultClass>, DefaultClass>);
 * @endcode
 *
 * @see DefaultDoCastIfPossible for usage in casting infrastructure
 */
template <typename OptionalDerived, typename Default>
using SelfType = std::conditional_t<std::is_same_v<OptionalDerived, void>, Default, OptionalDerived>;

}  // namespace corelib::detail

#endif  // LOGENIUM_CORELIB_DETAIL_CASTING_TRAITS_HXX
