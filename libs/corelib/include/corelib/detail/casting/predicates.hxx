// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_DETAIL_CASTING_PREDICATES_HXX
#define LOGENIUM_CORELIB_DETAIL_CASTING_PREDICATES_HXX

#include <corelib/casting/cast.hxx>
#include <corelib/casting/dyn_cast.hxx>
#include <corelib/casting/isa.hxx>

namespace corelib::detail {

template <typename... Types>
struct IsaCheckPredicate {
    template <typename T>
    bool operator()(const T &Val) const {
        return isa<Types...>(Val);
    }
};

template <typename... Types>
struct IsaAndPresentCheckPredicate {
    template <typename T>
    bool operator()(const T &Val) const {
        return isa_and_present<Types...>(Val);
    }
};

template <typename U>
struct StaticCastFunc {
    template <typename T>
    decltype(auto) operator()(T &&Val) const {
        return static_cast<U>(Val);
    }
};

template <typename U>
struct DynCastFunc {
    template <typename T>
    decltype(auto) operator()(T &&Val) const {
        return dyn_cast<U>(Val);
    }
};

template <typename U>
struct CastFunc {
    template <typename T>
    decltype(auto) operator()(T &&Val) const {
        return cast<U>(Val);
    }
};

template <typename U>
struct CastIfPresentFunc {
    template <typename T>
    decltype(auto) operator()(T &&Val) const {
        return cast_if_present<U>(Val);
    }
};

template <typename U>
struct DynCastIfPresentFunc {
    template <typename T>
    decltype(auto) operator()(T &&Val) const {
        return dyn_cast_if_present<U>(Val);
    }
};

}  // namespace corelib::detail

#endif  // LOGENIUM_CORELIB_DETAIL_CASTING_PREDICATES_HXX
