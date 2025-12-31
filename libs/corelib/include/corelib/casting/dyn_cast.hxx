// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_CASTING_DYN_CAST_HXX
#define LOGENIUM_CORELIB_CASTING_DYN_CAST_HXX

#include <memory>

#include <debug/assert.hxx>

#include <corelib/casting/cast.hxx>
#include <corelib/detail/casting/cast.hxx>
#include <corelib/detail/casting/traits.hxx>
#include <corelib/utility/type_name.hxx>

namespace corelib {

/**
 * @ingroup casting
 * @brief Dynamic cast that returns nullptr on type mismatch.
 *
 * dyn_cast() performs a type-safe downcast with runtime type checking.
 * Unlike cast(), it returns a null value (nullptr for pointers, empty for
 * unique_ptr) if the cast fails, rather than asserting. This provides a
 * safe way to attempt casts where the type is uncertain.
 *
 * Supported types:
 * - References (const and non-const) - returns reference or null reference (implementation defined)
 * - Pointers - returns pointer or nullptr
 * - std::unique_ptr - returns unique_ptr or empty unique_ptr
 *
 * Return type preserves const-correctness:
 * - const From& → const To& or null
 * - From& → To& or null
 * - From* → To* or nullptr
 * - unique_ptr<From>& → unique_ptr<To> or empty
 *
 * @tparam To The target type
 * @tparam From The source type (deduced)
 * @param Val The value to cast
 * @return Casted value if type matches, nullptr/null value otherwise
 *
 * @note Asserts if Val is null (use dyn_cast_or_null for null-safe version)
 * @note For pointers: returns nullptr on failure (safe to check)
 * @note For unique_ptr: transfers ownership only if cast succeeds
 * @note Uses decltype(auto) to preserve value categories
 *
 * Example:
 * @code
 * Base *base = getBase();
 *
 * // Safe cast - returns nullptr if not Derived
 * if (Derived *derived = dyn_cast<Derived>(base)) {
 *     // base was a Derived, use it
 * } else {
 *     // base was some other type
 * }
 *
 * // Works with unique_ptr
 * std::unique_ptr<Base> uptr = ...;
 * if (auto dptr = dyn_cast<Derived>(uptr)) {
 *     // Cast succeeded, ownership transferred
 * } else {
 *     // Cast failed, uptr still owns the object
 * }
 * @endcode
 *
 * @see cast() for checked cast with assertion
 * @see dyn_cast_or_null() for null-safe version
 * @see dyn_cast_if_present() for presence-checking version
 */
template <typename To, typename From>
inline decltype(auto) dyn_cast(const From &Val) {
    debug::Assert(detail::isPresent(Val), "dyn_cast<{}>(const {} &) called on a non-existent value", type_name<To>(),
                  type_name<From>());
    return detail::CastInfo<To, const From>::DoCastIfPossible(Val);
}

/// @copydoc dyn_cast(const From &)
template <typename To, typename From>
inline decltype(auto) dyn_cast(From &Val) {
    debug::Assert(detail::isPresent(Val), "dyn_cast<{}>({} &) called on a non-existent value", type_name<To>(),
                  type_name<From>());
    return detail::CastInfo<To, From>::DoCastIfPossible(Val);
}

/// @copydoc dyn_cast(const From &)
template <typename To, typename From>
inline decltype(auto) dyn_cast(From *Val) {
    debug::Assert(detail::isPresent(Val), "dyn_cast<{}>({}*) called on a non-existent value", type_name<To>(),
                  type_name<From>());
    return detail::CastInfo<To, From *>::DoCastIfPossible(Val);
}

/// @copydoc dyn_cast(const From &)
template <typename To, typename From>
inline decltype(auto) dyn_cast(std::unique_ptr<From> &Val) {
    debug::Assert(detail::isPresent(Val), "dyn_cast<{}>(std::unique_ptr<{}> &&) called on a non-existent value",
                  type_name<To>(), type_name<From>());
    return detail::CastInfo<To, std::unique_ptr<From>>::DoCastIfPossible(Val);
}

/**
 * @ingroup casting
 * @brief Null-safe dynamic cast with presence checking.
 *
 * dyn_cast_if_present() combines presence checking with dynamic casting.
 * It first checks if Val is present (not null/empty), then performs dyn_cast()
 * on the unwrapped value. Returns null/empty if Val is not present or if the
 * cast fails.
 *
 * This is the safest casting operation - it never asserts and handles both
 * null values and type mismatches gracefully.
 *
 * @tparam X The target type
 * @tparam Y The source type (deduced)
 * @param Val The value to check and cast
 * @return Casted value if present and correct type, null/empty otherwise
 *
 * @note No assertions - safe to call on null/empty values
 * @note Returns null if Val is not present (no type check performed)
 * @note Returns null if Val is present but wrong type
 *
 * Example:
 * @code
 * Base *base = ...;  // May be nullptr
 *
 * // Safe even if base is nullptr
 * if (Derived *derived = dyn_cast_if_present<Derived>(base)) {
 *     // base was non-null AND was a Derived
 * }
 *
 * std::optional<Base> opt = ...;
 * if (auto derived = dyn_cast_if_present<Derived>(opt)) {
 *     // opt had value AND was Derived
 * }
 * @endcode
 *
 * @see dyn_cast() for dynamic cast (asserts on null)
 * @see dyn_cast_or_null() for alias to this function
 */
template <class X, class Y>
auto dyn_cast_if_present(const Y &Val) {
    if (!detail::isPresent(Val)) return detail::CastInfo<X, const Y>::CastFailed();
    return detail::CastInfo<X, const Y>::DoCastIfPossible(detail::unwrapValue(Val));
}

/// @copydoc dyn_cast_if_present(const Y &)
template <class X, class Y>
auto dyn_cast_if_present(Y &Val) {
    if (!detail::isPresent(Val)) return detail::CastInfo<X, Y>::CastFailed();
    return detail::CastInfo<X, Y>::DoCastIfPossible(detail::unwrapValue(Val));
}

/// @copydoc dyn_cast_if_present(const Y &)
template <class X, class Y>
auto dyn_cast_if_present(Y *Val) {
    if (!detail::isPresent(Val)) return detail::CastInfo<X, Y *>::CastFailed();
    return detail::CastInfo<X, Y *>::DoCastIfPossible(detail::unwrapValue(Val));
}

/**
 * @ingroup casting
 * @brief Alias for dyn_cast_if_present() - null-safe dynamic cast.
 *
 * dyn_cast_or_null() is an alias for dyn_cast_if_present(), providing a more
 * intuitive name for pointer-based casting. It returns nullptr if the cast
 * fails or if the input is null.
 *
 * @tparam X The target type
 * @tparam Y The source type (deduced)
 * @param Val The value to check and cast
 * @return Casted value if present and correct type, nullptr otherwise
 *
 * @note Identical to dyn_cast_if_present()
 * @note No assertions - safe for null inputs and type mismatches
 *
 * Example:
 * @code
 * Base *base = ...;  // May be nullptr
 *
 * // Returns nullptr if base is null OR wrong type
 * Derived *derived = dyn_cast_or_null<Derived>(base);
 * if (derived) {
 *     // Safe to use
 * }
 * @endcode
 *
 * @see dyn_cast_if_present() for the underlying implementation
 */
template <class X, class Y>
auto dyn_cast_or_null(const Y &Val) {
    return dyn_cast_if_present<X>(Val);
}

/// @copydoc dyn_cast_or_null(const Y &)
template <class X, class Y>
auto dyn_cast_or_null(Y &Val) {
    return dyn_cast_if_present<X>(Val);
}

/// @copydoc dyn_cast_or_null(const Y &)
template <class X, class Y>
auto dyn_cast_or_null(Y *Val) {
    return dyn_cast_if_present<X>(Val);
}

/**
 * @ingroup casting
 * @brief Specialized dynamic cast for std::unique_ptr with ownership transfer.
 *
 * unique_dyn_cast() performs a type-safe cast of unique_ptr with ownership
 * semantics. If the cast succeeds, ownership is transferred to the returned
 * unique_ptr. If the cast fails, the original unique_ptr retains ownership
 * and an empty unique_ptr is returned.
 *
 * This function asserts that Val is non-null (use unique_dyn_cast_or_null
 * for null-safe version).
 *
 * @tparam X The target type
 * @tparam Y The source type (deduced)
 * @param Val The unique_ptr to cast (passed by reference or rvalue)
 * @return unique_ptr<X> if cast succeeds, empty unique_ptr otherwise
 *
 * @note Asserts if Val is null
 * @note Ownership transferred only if cast succeeds
 * @note Val becomes empty if cast succeeds, retains ownership if cast fails
 *
 * Example:
 * @code
 * std::unique_ptr<Base> base = getDerived();
 *
 * // Cast succeeds: ownership transferred
 * if (auto derived = unique_dyn_cast<Derived>(base)) {
 *     // base is now empty, derived owns the object
 * } else {
 *     // Cast failed, base still owns the object
 * }
 * @endcode
 *
 * @see dyn_cast() for general dynamic casting
 * @see unique_dyn_cast_or_null() for null-safe version
 */
template <class X, class Y>
inline typename detail::CastInfo<X, std::unique_ptr<Y>>::CastResultType unique_dyn_cast(std::unique_ptr<Y> &Val) {
    if (!isa<X>(Val)) return nullptr;
    return cast<X>(std::move(Val));
}

/// @copydoc unique_dyn_cast(std::unique_ptr<Y> &)
template <class X, class Y>
inline auto unique_dyn_cast(std::unique_ptr<Y> &&Val) {
    return unique_dyn_cast<X, Y>(Val);
}

/**
 * @ingroup casting
 * @brief Null-safe unique_ptr dynamic cast with ownership transfer.
 *
 * unique_dyn_cast_or_null() is the null-safe version of unique_dyn_cast().
 * It checks if Val is null before attempting the cast. If null, returns nullptr
 * immediately without assertion. Otherwise performs the dynamic cast.
 *
 * @tparam X The target type
 * @tparam Y The source type (deduced)
 * @param Val The unique_ptr to cast (may be null)
 * @return unique_ptr<X> if non-null and cast succeeds, nullptr otherwise
 *
 * @note No assertion - safe to call on null unique_ptr
 * @note Ownership transferred only if non-null and cast succeeds
 *
 * Example:
 * @code
 * std::unique_ptr<Base> base = ...;  // May be nullptr
 *
 * // Safe even if base is nullptr
 * if (auto derived = unique_dyn_cast_or_null<Derived>(base)) {
 *     // base was non-null AND was Derived, ownership transferred
 * } else {
 *     // base was null OR wrong type
 *     if (base) {
 *         // base still owns the object (wrong type)
 *     }
 * }
 * @endcode
 *
 * @see unique_dyn_cast() for version that asserts on null
 */
template <class X, class Y>
inline typename detail::CastInfo<X, std::unique_ptr<Y>>::CastResultType unique_dyn_cast_or_null(
    std::unique_ptr<Y> &Val) {
    if (!Val) return nullptr;
    return unique_dyn_cast<X, Y>(Val);
}

/// @copydoc unique_dyn_cast_or_null(std::unique_ptr<Y> &)
template <class X, class Y>
inline auto unique_dyn_cast_or_null(std::unique_ptr<Y> &&Val) {
    return unique_dyn_cast_or_null<X, Y>(Val);
}

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}  // namespace logenium

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_CASTING_DYN_CAST_HXX
