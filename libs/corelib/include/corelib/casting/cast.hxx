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

template <typename To, typename From>
inline decltype(auto) cast(const From &Val) {
    debug::Assert(isa<To>(Val), "cast<{}>(const {} &) arguments of incompatible type!", type_name<To>(),
                  type_name<From>());
    return detail::CastInfo<To, const From>::DoCast(Val);
}

template <typename To, typename From>
inline decltype(auto) cast(From &Val) {
    debug::Assert(isa<To>(Val), "cast<{}>({} &) arguments of incompatible type!", type_name<To>(), type_name<From>());
    return detail::CastInfo<To, From>::DoCast(Val);
}

template <typename To, typename From>
inline decltype(auto) cast(From *Val) {
    debug::Assert(isa<To>(Val), "cast<{}>({}*) arguments of incompatible type!", type_name<To>(), type_name<From>());
    return detail::CastInfo<To, From *>::DoCast(Val);
}

template <typename To, typename From>
inline decltype(auto) cast(std::unique_ptr<From> &&Val) {
    debug::Assert(isa<To>(Val), "cast<{}>(std::unique_ptr<{}> &&) arguments of incompatible type!", type_name<To>(),
                  type_name<From>());
    return detail::CastInfo<To, std::unique_ptr<From>>::DoCast(std::move(Val));
}

template <class X, class Y>
inline auto cast_if_present(const Y &Val) {
    if (!detail::isPresent(Val)) return detail::CastInfo<X, const Y>::CastFailed();
    debug::Assert(isa<X>(Val), "cast_if_present<{}>(const {} &) arguments of incompatible type!", type_name<X>(),
                  type_name<Y>());
    return cast<X>(detail::unwrapValue(Val));
}

template <class X, class Y>
inline auto cast_if_present(Y &Val) {
    if (!detail::isPresent(Val)) return detail::CastInfo<X, Y>::CastFailed();
    debug::Assert(isa<X>(Val), "cast_if_present<{}>({} &) arguments of incompatible type!", type_name<X>(),
                  type_name<Y>());
    return cast<X>(detail::unwrapValue(Val));
}

template <class X, class Y>
inline auto cast_if_present(Y *Val) {
    if (!detail::isPresent(Val)) return detail::CastInfo<X, Y *>::CastFailed();
    debug::Assert(isa<X>(Val), "cast_if_present<{}>({}*) arguments of incompatible type!", type_name<X>(),
                  type_name<Y>());
    return cast<X>(detail::unwrapValue(Val));
}

template <class X, class Y>
inline auto cast_if_present(std::unique_ptr<Y> &&Val) {
    if (!detail::isPresent(Val)) return detail::UniquePtrCast<X, Y>::CastFailed();
    return detail::UniquePtrCast<X, Y>::DoCast(std::move(Val));
}

template <class X, class Y>
auto cast_or_null(const Y &Val) {
    return cast_if_present<X>(Val);
}

template <class X, class Y>
auto cast_or_null(Y &Val) {
    return cast_if_present<X>(Val);
}

template <class X, class Y>
auto cast_or_null(Y *Val) {
    return cast_if_present<X>(Val);
}

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
