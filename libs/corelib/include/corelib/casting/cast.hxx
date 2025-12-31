// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_CASTING_CAST_HXX
#define LOGENIUM_CORELIB_CASTING_CAST_HXX

#include <memory>

#include <debug/assert.hxx>

#include <corelib/casting/isa.hxx>
#include <corelib/detail/casting/adapters.hxx>
#include <corelib/detail/casting/cast.hxx>
#include <corelib/detail/casting/traits.hxx>
#include <corelib/utility/type_name.hxx>

namespace corelib {

/**
 * @ingroup casting
 * @brief Checked cast with assertion on type mismatch.
 *
 * cast() performs a type-safe downcast with runtime type checking and assertion.
 * It first checks if the cast is valid using isa<To>(Val), then performs the cast.
 * If the type check fails, it asserts in debug builds.
 *
 * Supported types:
 * - References (const and non-const)
 * - Pointers
 * - std::unique_ptr (transfers ownership)
 *
 * Return type preserves const-correctness and reference/pointer semantics:
 * - const From& → const To&
 * - From& → To&
 * - From* → To*
 * - unique_ptr<From> → unique_ptr<To>
 *
 * @tparam To The target type
 * @tparam From The source type (deduced)
 * @param Val The value to cast
 * @return The casted value (type depends on input)
 *
 * @note Asserts if Val is not an instance of To (debug::Assert)
 * @note For pointers/unique_ptr, asserts Val is not null
 * @note For unique_ptr, ownership is transferred (Val becomes empty)
 * @note Uses decltype(auto) to preserve value categories
 *
 * Example:
 * @code
 * Base *base = getDerived();
 *
 * // Cast pointer (asserts if not Derived)
 * Derived *derived = cast<Derived>(base);
 *
 * // Cast reference
 * Base &ref = ...;
 * Derived &dref = cast<Derived>(ref);
 *
 * // Cast unique_ptr (transfers ownership)
 * std::unique_ptr<Base> uptr = ...;
 * std::unique_ptr<Derived> dptr = cast<Derived>(std::move(uptr));
 * // uptr is now empty
 *
 * // Preserves const
 * const Base *cbase = ...;
 * const Derived *cderived = cast<Derived>(cbase);
 * @endcode
 *
 * @see isa() for type checking without casting
 * @see dyn_cast() for casting that returns nullptr on failure
 * @see cast_if_present() for null-safe casting
 */
template <typename To, typename From>
inline decltype(auto) cast(const From &Val) {
    debug::Assert(isa<To>(Val), "cast<{}>(const {} &) arguments of incompatible type!", type_name<To>(),
                  type_name<From>());
    return detail::CastInfo<To, const From>::DoCast(Val);
}

/// @copydoc cast(const From &)
template <typename To, typename From>
inline decltype(auto) cast(From &Val) {
    debug::Assert(isa<To>(Val), "cast<{}>({} &) arguments of incompatible type!", type_name<To>(), type_name<From>());
    return detail::CastInfo<To, From>::DoCast(Val);
}

/// @copydoc cast(const From &)
template <typename To, typename From>
inline decltype(auto) cast(From *Val) {
    debug::Assert(isa<To>(Val), "cast<{}>({}*) arguments of incompatible type!", type_name<To>(), type_name<From>());
    return detail::CastInfo<To, From *>::DoCast(Val);
}

/// @copydoc cast(const From &)
template <typename To, typename From>
inline decltype(auto) cast(std::unique_ptr<From> &&Val) {
    debug::Assert(isa<To>(Val), "cast<{}>(std::unique_ptr<{}> &&) arguments of incompatible type!", type_name<To>(),
                  type_name<From>());
    return detail::CastInfo<To, std::unique_ptr<From>>::DoCast(std::move(Val));
}

/**
 * @ingroup casting
 * @brief Null-safe checked cast with assertion on type mismatch.
 *
 * cast_if_present() combines presence checking with checked casting. It first
 * checks if Val is present (not null/empty), returning a null value if absent.
 * If present, it unwraps the value and performs cast<X>() with assertion.
 *
 * Useful for optional types and nullable pointers where you want null-safety
 * but still want assertion on type mismatches for non-null values.
 *
 * Supported types:
 * - std::optional (checks has_value())
 * - Pointers (checks != nullptr)
 * - std::unique_ptr (checks != nullptr, transfers ownership)
 * - Regular types (always present)
 *
 * @tparam X The target type
 * @tparam Y The source type (deduced)
 * @param Val The value to check and cast
 * @return Casted value if present and correct type, null/empty otherwise
 *
 * @note Returns failure value if Val is not present (no assertion)
 * @note Asserts if Val is present but not of type X (debug::Assert)
 * @note For unique_ptr, only transfers ownership if present and correct type
 *
 * Example:
 * @code
 * std::optional<Base> opt = ...;
 *
 * // Returns std::nullopt if opt is empty, asserts if wrong type
 * auto result = cast_if_present<Derived>(opt);
 *
 * Base *ptr = ...;  // May be nullptr
 *
 * // Returns nullptr if ptr is null, asserts if wrong type
 * Derived *dptr = cast_if_present<Derived>(ptr);
 *
 * std::unique_ptr<Base> uptr = ...;
 * auto duptr = cast_if_present<Derived>(std::move(uptr));
 * // If uptr was null: duptr is null, uptr unchanged
 * // If uptr was non-null and correct type: ownership transferred
 * @endcode
 *
 * @see cast() for casting with assertion (no null check)
 * @see isa_and_present() for checking type and presence
 * @see cast_or_null() for alias to this function
 */
template <class X, class Y>
inline auto cast_if_present(const Y &Val) {
    if (!detail::isPresent(Val)) return detail::CastInfo<X, const Y>::CastFailed();
    debug::Assert(isa<X>(Val), "cast_if_present<{}>(const {} &) arguments of incompatible type!", type_name<X>(),
                  type_name<Y>());
    return cast<X>(detail::unwrapValue(Val));
}

/// @copydoc cast_if_present(const Y &)
template <class X, class Y>
inline auto cast_if_present(Y &Val) {
    if (!detail::isPresent(Val)) return detail::CastInfo<X, Y>::CastFailed();
    debug::Assert(isa<X>(Val), "cast_if_present<{}>({} &) arguments of incompatible type!", type_name<X>(),
                  type_name<Y>());
    return cast<X>(detail::unwrapValue(Val));
}

/// @copydoc cast_if_present(const Y &)
template <class X, class Y>
inline auto cast_if_present(Y *Val) {
    if (!detail::isPresent(Val)) return detail::CastInfo<X, Y *>::CastFailed();
    debug::Assert(isa<X>(Val), "cast_if_present<{}>({}*) arguments of incompatible type!", type_name<X>(),
                  type_name<Y>());
    return cast<X>(detail::unwrapValue(Val));
}

/// @copydoc cast_if_present(const Y &)
template <class X, class Y>
inline auto cast_if_present(std::unique_ptr<Y> &&Val) {
    if (!detail::isPresent(Val)) return detail::UniquePtrCast<X, Y>::CastFailed();
    return detail::UniquePtrCast<X, Y>::DoCast(std::move(Val));
}

/**
 * @ingroup casting
 * @brief Alias for cast_if_present() - null-safe checked cast.
 *
 * cast_or_null() is an alias for cast_if_present(), providing a more intuitive
 * name for pointer-based casting. It returns nullptr if the cast fails or if
 * the input is null.
 *
 * @tparam X The target type
 * @tparam Y The source type (deduced)
 * @param Val The value to check and cast
 * @return Casted value if present and correct type, null/empty otherwise
 *
 * @note Identical to cast_if_present()
 * @note Asserts if Val is non-null but not of type X
 *
 * Example:
 * @code
 * Base *base = ...;  // May be nullptr
 *
 * // Returns nullptr if base is null or wrong type (with assertion)
 * Derived *derived = cast_or_null<Derived>(base);
 *
 * if (derived) {
 *     // Safe to use derived
 * }
 * @endcode
 *
 * @see cast_if_present() for the underlying implementation
 */
template <class X, class Y>
auto cast_or_null(const Y &Val) {
    return cast_if_present<X>(Val);
}

/// @copydoc cast_or_null(const Y &)
template <class X, class Y>
auto cast_or_null(Y &Val) {
    return cast_if_present<X>(Val);
}

/// @copydoc cast_or_null(const Y &)
template <class X, class Y>
auto cast_or_null(Y *Val) {
    return cast_if_present<X>(Val);
}

/// @copydoc cast_or_null(const Y &)
template <class X, class Y>
auto cast_or_null(std::unique_ptr<Y> &&Val) {
    return cast_if_present<X>(std::move(Val));
}

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_CASTING_CAST_HXX
