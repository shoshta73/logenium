// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_CASTING_ISA_HXX
#define LOGENIUM_CORELIB_CASTING_ISA_HXX

#include <corelib/detail/casting/cast.hxx>
#include <corelib/detail/casting/traits.hxx>

namespace corelib {

template <typename... To, typename From>
inline bool isa(const From &Val) {
    return (detail::CastInfo<To, const From>::IsPossible(Val) || ...);
}

template <typename... X, class Y>
inline bool isa_and_present(const Y &Val) {
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
