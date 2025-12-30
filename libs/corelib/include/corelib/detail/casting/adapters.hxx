// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_DETAIL_CASTING_ADAPTERS_HXX
#define LOGENIUM_CORELIB_DETAIL_CASTING_ADAPTERS_HXX

#include <memory>
#include <optional>
#include <type_traits>

#include <corelib/detail/casting/forwarding.hxx>
#include <corelib/detail/casting/traits.hxx>

namespace corelib::detail {

// Forward declarations to avoid circular dependency with cast.hxx
template <typename To, typename From, typename Enable = void>
struct CastIsPossible;

template <class To, class From>
struct CastRetty;

// =============================================================================
// Section 1: Unique Pointer Casting
// =============================================================================

template <typename To, typename From, typename Derived = void>
struct UniquePtrCast : CastIsPossible<To, From *> {
    using Self = SelfType<Derived, UniquePtrCast<To, From>>;
    using CastResultType = std::unique_ptr<std::remove_reference_t<typename CastRetty<To, From>::ret_type>>;

    static inline CastResultType DoCast(std::unique_ptr<From> &&f) {
        return CastResultType((typename CastResultType::element_type *)f.release());
    }

    static inline CastResultType CastFailed() { return CastResultType(nullptr); }

    static inline CastResultType DoCastIfPossible(std::unique_ptr<From> &f) {
        if (!Self::IsPossible(f.get())) return CastFailed();
        return DoCast(std::move(f));
    }
};

// =============================================================================
// Section 2: Optional Casting
// =============================================================================

template <typename To, typename From, typename Derived = void>
struct OptionalValueCast
    : public CastIsPossible<To, From>,
      public DefaultDoCastIfPossible<std::optional<To>, From, SelfType<Derived, OptionalValueCast<To, From>>> {
    static inline std::optional<To> CastFailed() { return std::optional<To>{}; }

    static inline std::optional<To> DoCast(const From &f) { return To(f); }
};

// =============================================================================
// Section 3: Pointer-to-Value Casting
// =============================================================================

template <typename To, typename From, typename Derived = void>
struct ValueFromPointerCast
    : public CastIsPossible<To, From *>,
      public NullableValueCastFailed<To>,
      public DefaultDoCastIfPossible<To, From *, SelfType<Derived, ValueFromPointerCast<To, From>>> {
    static inline To DoCast(From *f) { return To(f); }
};

}  // namespace corelib::detail

#endif  // LOGENIUM_CORELIB_DETAIL_CASTING_ADAPTERS_HXX
