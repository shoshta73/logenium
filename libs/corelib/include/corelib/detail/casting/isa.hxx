// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_DETAIL_CASTING_ISA_HXX
#define LOGENIUM_CORELIB_DETAIL_CASTING_ISA_HXX

#include <memory>
#include <type_traits>

#include <debug/assert.hxx>

#include <corelib/detail/casting/traits.hxx>
#include <corelib/utility/type_name.hxx>

namespace corelib::detail {

// =============================================================================
// Section 1: Base Isa Implementation
// =============================================================================

template <typename To, typename From, typename Enabler = void>
struct IsaImpl {
    static inline bool Check(const From &Val) { return To::classof(&Val); }
};

template <typename To, typename From>
struct IsaImpl<To, From, std::enable_if_t<std::is_base_of_v<To, From>>> {
    static inline bool Check(const From &) { return true; }
};

// =============================================================================
// Section 2: Const and Pointer Specializations
// =============================================================================

template <typename To, typename From>
struct IsaImplConst {
    static inline bool Check(const From &Val) { return IsaImpl<To, From>::Check(Val); }
};

template <typename To, typename From>
struct IsaImplConst<To, const From> {
    static inline bool Check(const From &Val) { return IsaImpl<To, From>::Check(Val); }
};

template <typename To, typename From>
struct IsaImplConst<To, const std::unique_ptr<From>> {
    static inline bool Check(const std::unique_ptr<From> &Val) {
        debug::Assert(Val.get(), "isa<{}>(const std::unique_ptr<{}> &) called on a null pointer", type_name<To>(),
                      type_name<From>());
        return IsaImplConst<To, From>::Check(*Val);
    }
};

template <typename To, typename From>
struct IsaImplConst<To, From *> {
    static inline bool Check(const From *Val) {
        debug::Assert(Val, "isa<{}>({}*) called on a null pointer", type_name<To>(), type_name<From>());
        return IsaImpl<To, From>::Check(*Val);
    }
};

template <typename To, typename From>
struct IsaImplConst<To, From *const> {
    static inline bool Check(const From *Val) {
        debug::Assert(Val, "isa<{}>(const {}*) called on a null pointer", type_name<To>(), type_name<From>());
        return IsaImpl<To, From>::Check(*Val);
    }
};

template <typename To, typename From>
struct IsaImplConst<To, const From *> {
    static inline bool Check(const From *Val) {
        debug::Assert(Val, "isa<{}>(const {} *) called on a null pointer", type_name<To>(), type_name<From>());
        return IsaImpl<To, From>::Check(*Val);
    }
};

template <typename To, typename From>
struct IsaImplConst<To, const From *const> {
    static inline bool Check(const From *Val) {
        debug::Assert(Val, "isa<{}>(const {} *) called on a null pointer", type_name<To>(), type_name<From>());
        return IsaImpl<To, From>::Check(*Val);
    }
};

// =============================================================================
// Section 3: Type Simplification Wrapper
// =============================================================================

template <typename To, typename From, typename SimpleFrom>
struct IsaImplWrap {
    static bool Check(const From &Val) {
        return IsaImplWrap<To, SimpleFrom, typename SimplifyType<SimpleFrom>::SimpleType>::Check(
            SimplifyType<const From>::GetSimplifiedValue(Val));
    }
};

template <typename To, typename FromTy>
struct IsaImplWrap<To, FromTy, FromTy> {
    static bool Check(const FromTy &Val) { return IsaImplConst<To, FromTy>::Check(Val); }
};

}  // namespace corelib::detail

#endif  // LOGENIUM_CORELIB_DETAIL_CASTING_ISA_HXX
