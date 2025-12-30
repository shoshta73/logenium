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

template <typename To, typename From>
inline decltype(auto) dyn_cast(const From &Val) {
    debug::Assert(detail::isPresent(Val), "dyn_cast<{}>(const {} &) called on a non-existent value", type_name<To>(),
                  type_name<From>());
    return detail::CastInfo<To, const From>::DoCastIfPossible(Val);
}

template <typename To, typename From>
inline decltype(auto) dyn_cast(From &Val) {
    debug::Assert(detail::isPresent(Val), "dyn_cast<{}>({} &) called on a non-existent value", type_name<To>(),
                  type_name<From>());
    return detail::CastInfo<To, From>::DoCastIfPossible(Val);
}

template <typename To, typename From>
inline decltype(auto) dyn_cast(From *Val) {
    debug::Assert(detail::isPresent(Val), "dyn_cast<{}>({}*) called on a non-existent value", type_name<To>(),
                  type_name<From>());
    return detail::CastInfo<To, From *>::DoCastIfPossible(Val);
}

template <typename To, typename From>
inline decltype(auto) dyn_cast(std::unique_ptr<From> &Val) {
    debug::Assert(detail::isPresent(Val), "dyn_cast<{}>(std::unique_ptr<{}> &&) called on a non-existent value",
                  type_name<To>(), type_name<From>());
    return detail::CastInfo<To, std::unique_ptr<From>>::DoCastIfPossible(Val);
}

template <class X, class Y>
auto dyn_cast_if_present(const Y &Val) {
    if (!detail::isPresent(Val)) return detail::CastInfo<X, const Y>::CastFailed();
    return detail::CastInfo<X, const Y>::DoCastIfPossible(detail::unwrapValue(Val));
}

template <class X, class Y>
auto dyn_cast_if_present(Y &Val) {
    if (!detail::isPresent(Val)) return detail::CastInfo<X, Y>::CastFailed();
    return detail::CastInfo<X, Y>::DoCastIfPossible(detail::unwrapValue(Val));
}

template <class X, class Y>
auto dyn_cast_if_present(Y *Val) {
    if (!detail::isPresent(Val)) return detail::CastInfo<X, Y *>::CastFailed();
    return detail::CastInfo<X, Y *>::DoCastIfPossible(detail::unwrapValue(Val));
}

template <class X, class Y>
auto dyn_cast_or_null(const Y &Val) {
    return dyn_cast_if_present<X>(Val);
}

template <class X, class Y>
auto dyn_cast_or_null(Y &Val) {
    return dyn_cast_if_present<X>(Val);
}

template <class X, class Y>
auto dyn_cast_or_null(Y *Val) {
    return dyn_cast_if_present<X>(Val);
}

template <class X, class Y>
inline typename detail::CastInfo<X, std::unique_ptr<Y>>::CastResultType unique_dyn_cast(std::unique_ptr<Y> &Val) {
    if (!isa<X>(Val)) return nullptr;
    return cast<X>(std::move(Val));
}

template <class X, class Y>
inline auto unique_dyn_cast(std::unique_ptr<Y> &&Val) {
    return unique_dyn_cast<X, Y>(Val);
}

template <class X, class Y>
inline typename detail::CastInfo<X, std::unique_ptr<Y>>::CastResultType unique_dyn_cast_or_null(
    std::unique_ptr<Y> &Val) {
    if (!Val) return nullptr;
    return unique_dyn_cast<X, Y>(Val);
}

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
