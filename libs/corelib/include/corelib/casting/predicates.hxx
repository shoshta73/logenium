// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_CASTING_PREDICATES_HXX
#define LOGENIUM_CORELIB_CASTING_PREDICATES_HXX

#include <corelib/detail/casting/predicates.hxx>

namespace corelib {

template <typename... Types>
inline constexpr detail::IsaCheckPredicate<Types...> IsaPred{};

template <typename... Types>
inline constexpr detail::IsaAndPresentCheckPredicate<Types...> IsaAndPresentPred{};

template <typename To>
inline constexpr detail::StaticCastFunc<To> StaticCastTo{};

template <typename To>
inline constexpr detail::CastFunc<To> CastTo{};

template <typename To>
inline constexpr detail::CastIfPresentFunc<To> CastIfPresentTo{};

template <typename To>
inline constexpr detail::DynCastIfPresentFunc<To> DynCastIfPresentTo{};

template <typename To>
inline constexpr detail::DynCastFunc<To> DynCastTo{};

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_CASTING_PREDICATES_HXX
