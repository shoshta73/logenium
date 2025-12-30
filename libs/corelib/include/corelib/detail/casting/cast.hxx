// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_DETAIL_CASTING_CAST_HXX
#define LOGENIUM_CORELIB_DETAIL_CASTING_CAST_HXX

#include <memory>
#include <optional>
#include <type_traits>

#include <corelib/detail/casting/adapters.hxx>
#include <corelib/detail/casting/isa.hxx>
#include <corelib/detail/casting/traits.hxx>

namespace corelib::detail {

// =============================================================================
// Section 1: Return Type Calculation
// =============================================================================

// Forward declaration
template <class To, class From>
struct CastRetty;

// Return type implementation for different input types
template <class To, class From>
struct CastRettyImpl {
    using ret_type = To &;
};

template <class To, class From>
struct CastRettyImpl<To, const From> {
    using ret_type = const To &;
};

template <class To, class From>
struct CastRettyImpl<To, From *> {
    using ret_type = To *;
};

template <class To, class From>
struct CastRettyImpl<To, const From *> {
    using ret_type = const To *;
};

template <class To, class From>
struct CastRettyImpl<To, const From *const> {
    using ret_type = const To *;
};

template <class To, class From>
struct CastRettyImpl<To, std::unique_ptr<From>> {
  private:
    using PointerType = typename CastRettyImpl<To, From *>::ret_type;
    using ResultType = std::remove_pointer_t<PointerType>;

  public:
    using ret_type = std::unique_ptr<ResultType>;
};

// Return type wrapper with type simplification
template <class To, class From, class SimpleFrom>
struct CastRettyWrap {
    using ret_type = typename CastRetty<To, SimpleFrom>::ret_type;
};

template <class To, class FromTy>
struct CastRettyWrap<To, FromTy, FromTy> {
    using ret_type = typename CastRettyImpl<To, FromTy>::ret_type;
};

// Main CastRetty definition
template <class To, class From>
struct CastRetty {
    using ret_type = typename CastRettyWrap<To, From, typename SimplifyType<From>::SimpleType>::ret_type;
};

// =============================================================================
// Section 2: Cast Possibility Checks
// =============================================================================

template <typename To, typename From, typename Enable>
struct CastIsPossible {
    static inline bool IsPossible(const From &f) {
        return detail::IsaImplWrap<To, const From, typename detail::SimplifyType<const From>::SimpleType>::Check(f);
    }
};

template <typename To, typename From>
struct CastIsPossible<To, std::optional<From>> {
    static inline bool IsPossible(const std::optional<From> &f) {
        assert(f && "CastIsPossible::IsPossible called on a nullopt!");
        return detail::IsaImplWrap<To, const From, typename detail::SimplifyType<const From>::SimpleType>::Check(*f);
    }
};

template <typename To, typename From>
struct CastIsPossible<To, From, std::enable_if_t<std::is_base_of_v<To, From>>> {
    static inline bool IsPossible(const From &f) { return true; }
};

// =============================================================================
// Section 3: Cast Conversion
// =============================================================================

template <class To, class From, class SimpleFrom>
struct CastConvertVal {
    static typename CastRetty<To, From>::ret_type Convert(const From &Val) {
        return CastConvertVal<To, SimpleFrom, typename SimplifyType<SimpleFrom>::SimpleType>::Convert(
            SimplifyType<From>::GetSimplifiedValue(const_cast<From &>(Val)));
    }
};

template <class To, class FromTy>
struct CastConvertVal<To, FromTy, FromTy> {
    static typename CastRetty<To, FromTy>::ret_type Convert(const FromTy &Val) {
        return *(std::remove_reference_t<typename CastRetty<To, FromTy>::ret_type> *)&const_cast<FromTy &>(Val);
    }
};

template <class To, class FromTy>
struct CastConvertVal<To, FromTy *, FromTy *> {
    static typename CastRetty<To, FromTy *>::ret_type Convert(const FromTy *Val) {
        return (typename CastRetty<To, FromTy *>::ret_type) const_cast<FromTy *>(Val);
    }
};

// =============================================================================
// Section 4: Main CastInfo Orchestrator
// =============================================================================

template <typename To, typename From, typename Enable = void>
struct CastInfo : CastIsPossible<To, From> {
    using Self = CastInfo<To, From, Enable>;

    using CastReturnType = typename CastRetty<To, From>::ret_type;

    static inline CastReturnType DoCast(const From &f) {
        return CastConvertVal<To, From, typename SimplifyType<From>::SimpleType>::Convert(const_cast<From &>(f));
    }

    static inline CastReturnType CastFailed() { return CastReturnType(nullptr); }

    static inline CastReturnType DoCastIfPossible(const From &f) {
        if (!Self::IsPossible(f)) return CastFailed();
        return DoCast(f);
    }
};

template <typename To, typename From>
struct CastInfo<To, From, std::enable_if_t<!IsSimpleType<From>::value>> {
    using Self = CastInfo<To, From>;
    using SimpleFrom = typename SimplifyType<From>::SimpleType;
    using SimplifiedSelf = CastInfo<To, SimpleFrom>;

    static inline bool IsPossible(From &f) {
        return SimplifiedSelf::IsPossible(SimplifyType<From>::GetSimplifiedValue(f));
    }

    static inline decltype(auto) DoCast(From &f) {
        return SimplifiedSelf::DoCast(SimplifyType<From>::GetSimplifiedValue(f));
    }

    static inline decltype(auto) CastFailed() { return SimplifiedSelf::CastFailed(); }

    static inline decltype(auto) DoCastIfPossible(From &f) {
        return SimplifiedSelf::DoCastIfPossible(SimplifyType<From>::GetSimplifiedValue(f));
    }
};

template <typename To, typename From>
struct CastInfo<To, std::unique_ptr<From>> : public UniquePtrCast<To, From> {};

template <typename To, typename From>
struct CastInfo<To, std::optional<From>> : OptionalValueCast<To, From> {};

}  // namespace corelib::detail

#endif  // LOGENIUM_CORELIB_DETAIL_CASTING_CAST_HXX
