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

// TODO: rename this and/or replace it with other cast traits.
template <typename From>
struct SimplifyType {
    using SimpleType = From;

    static SimpleType &GetSimplifiedValue(From &Val) { return Val; }
};

template <typename From>
struct SimplifyType<const From> {
    using NonConstSimpleType = typename SimplifyType<From>::SimpleType;
    using SimpleType = add_const_past_pointer_t<NonConstSimpleType>;
    using RetType = add_lvalue_reference_if_not_pointer_t<SimpleType>;

    static RetType GetSimplifiedValue(const From &Val) {
        return SimplifyType<From>::GetSimplifiedValue(const_cast<From &>(Val));
    }
};

template <class X>
struct IsSimpleType {
    static const bool value = std::is_same_v<X, typename SimplifyType<X>::SimpleType>;
};

// =============================================================================
// Section 2: Nullable Type Detection
// =============================================================================

template <typename T>
constexpr bool IsNullable = std::is_pointer_v<T> || std::is_constructible_v<T, std::nullptr_t>;

// =============================================================================
// Section 3: Value Presence Detection
// =============================================================================

template <typename T, typename Enable = void>
struct ValueIsPresent {
    using UnwrappedType = T;
    static inline bool IsPresent(const T &t) { return true; }
    static inline decltype(auto) UnwrapValue(T &t) { return t; }
};

template <typename T>
struct ValueIsPresent<std::optional<T>> {
    using UnwrappedType = T;
    static inline bool IsPresent(const std::optional<T> &t) { return t.has_value(); }
    static inline decltype(auto) UnwrapValue(std::optional<T> &t) { return *t; }
};

template <typename T>
struct ValueIsPresent<T, std::enable_if_t<detail::IsNullable<T>>> {
    using UnwrappedType = T;
    static inline bool IsPresent(const T &t) { return t != T(nullptr); }
    static inline decltype(auto) UnwrapValue(T &t) { return t; }
};

template <typename T>
inline bool isPresent(const T &t) {
    return ValueIsPresent<typename SimplifyType<T>::SimpleType>::IsPresent(
        SimplifyType<T>::GetSimplifiedValue(const_cast<T &>(t)));
}

template <typename T>
inline decltype(auto) unwrapValue(T &t) {
    return ValueIsPresent<T>::UnwrapValue(t);
}

// =============================================================================
// Section 4: Utilities
// =============================================================================

template <typename OptionalDerived, typename Default>
using SelfType = std::conditional_t<std::is_same_v<OptionalDerived, void>, Default, OptionalDerived>;

}  // namespace corelib::detail

#endif  // LOGENIUM_CORELIB_DETAIL_CASTING_TRAITS_HXX
