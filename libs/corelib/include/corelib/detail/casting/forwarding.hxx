// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_DETAIL_CASTING_FORWARDING_HXX
#define LOGENIUM_CORELIB_DETAIL_CASTING_FORWARDING_HXX

#include <type_traits>

namespace corelib::detail {

// =============================================================================
// Section 1: Default Cast Failed Implementations
// =============================================================================

template <typename To>
struct NullableValueCastFailed {
    static To CastFailed() { return To(nullptr); }
};

template <typename To, typename From, typename Derived>
struct DefaultDoCastIfPossible {
    static To DoCastIfPossible(From f) {
        if (!Derived::IsPossible(f)) return Derived::CastFailed();
        return Derived::DoCast(f);
    }
};

// =============================================================================
// Section 2: Forwarding Strategies
// =============================================================================

template <typename To, typename From, typename ForwardTo>
struct ForwardToPointerCast {
    static inline bool IsPossible(const From &f) { return ForwardTo::IsPossible(&f); }

    static inline decltype(auto) DoCast(const From &f) { return *ForwardTo::DoCast(&f); }
};

template <typename To, typename From, typename ForwardTo>
struct ConstStrippingForwardingCast {
    using DecayedFrom = std::remove_cv_t<std::remove_pointer_t<From>>;
    using NonConstFrom = std::conditional_t<std::is_pointer_v<From>, DecayedFrom *, DecayedFrom &>;

    static inline bool IsPossible(const From &f) { return ForwardTo::IsPossible(const_cast<NonConstFrom>(f)); }

    static inline decltype(auto) CastFailed() { return ForwardTo::CastFailed(); }

    static inline decltype(auto) DoCast(const From &f) { return ForwardTo::DoCast(const_cast<NonConstFrom>(f)); }

    static inline decltype(auto) DoCastIfPossible(const From &f) {
        return ForwardTo::DoCastIfPossible(const_cast<NonConstFrom>(f));
    }
};

}  // namespace corelib::detail

#endif  // LOGENIUM_CORELIB_DETAIL_CASTING_FORWARDING_HXX
