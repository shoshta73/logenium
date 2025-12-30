// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_CASTING_CAST_HXX
#define LOGENIUM_CORELIB_CASTING_CAST_HXX

#include <memory>

#include <corelib/casting/isa.hxx>
#include <corelib/detail/casting/adapters.hxx>
#include <corelib/detail/casting/cast.hxx>
#include <corelib/detail/casting/traits.hxx>

namespace corelib {

template <typename To, typename From>
inline decltype(auto) cast(const From &Val) {
    assert(isa<To>(Val) && "cast<Ty>() argument of incompatible type!");
    return detail::CastInfo<To, const From>::DoCast(Val);
}

template <typename To, typename From>
inline decltype(auto) cast(From &Val) {
    assert(isa<To>(Val) && "cast<Ty>() argument of incompatible type!");
    return detail::CastInfo<To, From>::DoCast(Val);
}

template <typename To, typename From>
inline decltype(auto) cast(From *Val) {
    assert(isa<To>(Val) && "cast<Ty>() argument of incompatible type!");
    return detail::CastInfo<To, From *>::DoCast(Val);
}

template <typename To, typename From>
inline decltype(auto) cast(std::unique_ptr<From> &&Val) {
    assert(isa<To>(Val) && "cast<Ty>() argument of incompatible type!");
    return detail::CastInfo<To, std::unique_ptr<From>>::DoCast(std::move(Val));
}

template <class X, class Y>
inline auto cast_if_present(const Y &Val) {
    if (!detail::isPresent(Val)) return detail::CastInfo<X, const Y>::CastFailed();
    assert(isa<X>(Val) && "cast_if_present<Ty>() argument of incompatible type!");
    return cast<X>(detail::unwrapValue(Val));
}

template <class X, class Y>
inline auto cast_if_present(Y &Val) {
    if (!detail::isPresent(Val)) return detail::CastInfo<X, Y>::CastFailed();
    assert(isa<X>(Val) && "cast_if_present<Ty>() argument of incompatible type!");
    return cast<X>(detail::unwrapValue(Val));
}

template <class X, class Y>
inline auto cast_if_present(Y *Val) {
    if (!detail::isPresent(Val)) return detail::CastInfo<X, Y *>::CastFailed();
    assert(isa<X>(Val) && "cast_if_present<Ty>() argument of incompatible type!");
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
