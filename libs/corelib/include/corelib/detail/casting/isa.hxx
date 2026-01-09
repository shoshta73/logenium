// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_DETAIL_CASTING_ISA_HXX
#define LOGENIUM_CORELIB_DETAIL_CASTING_ISA_HXX

#include <memory>
#include <type_traits>

#include <debug/assert.hxx>

#include <corelib/detail/casting/traits.hxx>
#include <corelib/internal/tracing.hxx>
#include <corelib/utility/type_name.hxx>

namespace corelib::detail {

// =============================================================================
// Section 1: Base Isa Implementation
// =============================================================================

/**
 * @ingroup casting_detail
 * @brief Base template for RTTI-free type checking using classof() pattern.
 *
 * IsaImpl provides the core type-checking mechanism for the casting system.
 * It delegates to the target type's static classof() method, implementing the
 * LLVM-style RTTI-free type checking pattern. Types must provide a classof()
 * method that returns true if a given pointer is of the target type.
 *
 * The base template handles the general case by calling To::classof(&Val).
 * A specialization handles the trivial case where From derives from To,
 * which is always true and doesn't require classof().
 *
 * @tparam To The target type to check for
 * @tparam From The source type being checked
 * @tparam Enabler SFINAE enabler for specializations
 *
 * ## Static Methods
 *
 * - `Check(const From &Val)`: Returns true if Val is of type To (via classof())
 *
 * @note Requires To to have a static classof(const From*) method
 * @note This is the foundation of isa<To>(Val) functionality
 *
 * Example:
 * @code
 * struct Base {
 *     virtual ~Base() = default;
 *     static bool classof(const Base *) { return true; }
 * };
 *
 * struct Derived : Base {
 *     static bool classof(const Base *b) {
 *         // Check if b is actually a Derived instance
 *         return; //type check logic
 *     }
 * };
 *
 * Derived d;
 * bool result = IsaImpl<Derived, Base>::Check(d);  // Calls Derived::classof(&d)
 * @endcode
 *
 * @see IsaImplConst for const and pointer handling
 * @see IsaImplWrap for type simplification integration
 */
template <typename To, typename From, typename Enabler = void>
struct IsaImpl {
    static inline bool Check(const From &Val) {
        CRLB_ZONE_SCOPED;
        return To::classof(&Val);
    }
};

/**
 * @ingroup casting_detail
 * @brief Specialization for when From is derived from To (always true).
 *
 * This specialization handles the trivial case where the source type From
 * is derived from the target type To. In this case, the check always succeeds
 * without needing to call classof(), since every Derived instance is also a Base.
 *
 * @tparam To The base type
 * @tparam From The derived type (must derive from To)
 *
 * ## Static Methods
 *
 * - `Check(const From &)`: Always returns true
 *
 * @note Enabled via SFINAE when std::is_base_of_v<To, From> is true
 * @note Optimization - avoids classof() call for trivial upcasts
 *
 * Example:
 * @code
 * struct Base { };
 * struct Derived : Base { };
 *
 * Derived d;
 * bool result = IsaImpl<Base, Derived>::Check(d);  // Always true (no classof call)
 * @endcode
 */
template <typename To, typename From>
struct IsaImpl<To, From, std::enable_if_t<std::is_base_of_v<To, From>>> {
    static inline bool Check(const From &) {
        CRLB_ZONE_SCOPED;
        return true;
    }
};

// =============================================================================
// Section 2: Const and Pointer Specializations
// =============================================================================

/**
 * @ingroup casting_detail
 * @brief Base template for const-correct type checking with pointer support.
 *
 * IsaImplConst extends IsaImpl with const-correctness and pointer/unique_ptr
 * handling. It provides specializations for various const-qualified types and
 * dereferences pointers before delegating to IsaImpl.
 *
 * The base template simply forwards to IsaImpl for non-pointer, non-const types.
 * Specializations handle:
 * - const-qualified types
 * - Pointer types (with null checking)
 * - std::unique_ptr (with null checking)
 * - Various combinations of const and pointer
 *
 * @tparam To The target type to check for
 * @tparam From The source type (may include const/pointer qualifiers)
 *
 * ## Static Methods
 *
 * - `Check(const From &Val)`: Returns true if Val is of type To
 *
 * @note Base template forwards to IsaImpl
 * @note Pointer specializations dereference and assert non-null
 * @note Const specializations strip const and forward to non-const implementation
 *
 * Example:
 * @code
 * Derived d;
 * Derived *ptr = &d;
 *
 * // Base template (non-const reference)
 * bool r1 = IsaImplConst<Base, Derived>::Check(d);
 *
 * // Const specialization
 * bool r2 = IsaImplConst<Base, const Derived>::Check(d);
 *
 * // Pointer specialization (dereferences ptr)
 * bool r3 = IsaImplConst<Base, Derived*>::Check(ptr);
 * @endcode
 *
 * @see IsaImpl for the underlying type-checking mechanism
 */
template <typename To, typename From>
struct IsaImplConst {
    static inline bool Check(const From &Val) {
        CRLB_ZONE_SCOPED;
        return IsaImpl<To, From>::Check(Val);
    }
};

/**
 * @ingroup casting_detail
 * @brief Specialization for const-qualified types.
 *
 * Strips const from the source type and forwards to IsaImpl. This ensures
 * const-qualified values can be type-checked using the same logic as non-const.
 *
 * @tparam To The target type
 * @tparam From The const-qualified source type
 */
template <typename To, typename From>
struct IsaImplConst<To, const From> {
    static inline bool Check(const From &Val) {
        CRLB_ZONE_SCOPED;
        return IsaImpl<To, From>::Check(Val);
    }
};

/**
 * @ingroup casting_detail
 * @brief Specialization for const std::unique_ptr.
 *
 * Dereferences the unique_ptr and recursively checks the pointed-to type.
 * Asserts that the unique_ptr is not null before dereferencing.
 *
 * @tparam To The target type
 * @tparam From The type contained in the unique_ptr
 *
 * @note Asserts Val.get() != nullptr using debug::Assert
 */
template <typename To, typename From>
struct IsaImplConst<To, const std::unique_ptr<From>> {
    static inline bool Check(const std::unique_ptr<From> &Val) {
        CRLB_ZONE_SCOPED;
        debug::Assert(Val.get(), "isa<{}>(const std::unique_ptr<{}> &) called on a null pointer", type_name<To>(),
                      type_name<From>());
        return IsaImplConst<To, From>::Check(*Val);
    }
};

/**
 * @ingroup casting_detail
 * @brief Specialization for non-const pointer (From*).
 *
 * Dereferences the pointer and forwards to IsaImpl. Asserts non-null before
 * dereferencing.
 *
 * @tparam To The target type
 * @tparam From The pointed-to type
 *
 * @note Asserts Val != nullptr using debug::Assert
 */
template <typename To, typename From>
struct IsaImplConst<To, From *> {
    static inline bool Check(const From *Val) {
        CRLB_ZONE_SCOPED;
        debug::Assert(Val, "isa<{}>({}*) called on a null pointer", type_name<To>(), type_name<From>());
        return IsaImpl<To, From>::Check(*Val);
    }
};

/**
 * @ingroup casting_detail
 * @brief Specialization for const pointer to non-const (From* const).
 *
 * The pointer itself is const, but the pointee is not. Dereferences and forwards.
 *
 * @tparam To The target type
 * @tparam From The pointed-to type
 *
 * @note Asserts Val != nullptr using debug::Assert
 */
template <typename To, typename From>
struct IsaImplConst<To, From *const> {
    static inline bool Check(const From *Val) {
        CRLB_ZONE_SCOPED;
        debug::Assert(Val, "isa<{}>(const {}*) called on a null pointer", type_name<To>(), type_name<From>());
        return IsaImpl<To, From>::Check(*Val);
    }
};

/**
 * @ingroup casting_detail
 * @brief Specialization for pointer to const (const From*).
 *
 * The pointee is const. Dereferences and forwards to IsaImpl.
 *
 * @tparam To The target type
 * @tparam From The const-qualified pointed-to type
 *
 * @note Asserts Val != nullptr using debug::Assert
 */
template <typename To, typename From>
struct IsaImplConst<To, const From *> {
    static inline bool Check(const From *Val) {
        CRLB_ZONE_SCOPED;
        debug::Assert(Val, "isa<{}>(const {} *) called on a null pointer", type_name<To>(), type_name<From>());
        return IsaImpl<To, From>::Check(*Val);
    }
};

/**
 * @ingroup casting_detail
 * @brief Specialization for const pointer to const (const From* const).
 *
 * Both pointer and pointee are const. Dereferences and forwards.
 *
 * @tparam To The target type
 * @tparam From The const-qualified pointed-to type
 *
 * @note Asserts Val != nullptr using debug::Assert
 */
template <typename To, typename From>
struct IsaImplConst<To, const From *const> {
    static inline bool Check(const From *Val) {
        CRLB_ZONE_SCOPED;
        debug::Assert(Val, "isa<{}>(const {} *) called on a null pointer", type_name<To>(), type_name<From>());
        return IsaImpl<To, From>::Check(*Val);
    }
};

// =============================================================================
// Section 3: Type Simplification Wrapper
// =============================================================================

/**
 * @ingroup casting_detail
 * @brief Recursive type simplification wrapper for type checking.
 *
 * IsaImplWrap integrates SimplifyType's recursive type unwrapping with the
 * type-checking infrastructure. It recursively simplifies the From type using
 * SimplifyType until it reaches a "simple" type (one that SimplifyType doesn't
 * further simplify), then delegates to IsaImplConst for the actual check.
 *
 * This template provides the recursive case, continuing to simplify until
 * From == SimpleFrom (handled by the specialization).
 *
 * @tparam To The target type to check for
 * @tparam From The original source type (potentially with qualifiers)
 * @tparam SimpleFrom The simplified version of From (SimplifyType<From>::SimpleType)
 *
 * ## Static Methods
 *
 * - `Check(const From &Val)`: Recursively simplifies Val and checks type
 *
 * @note Recursive template - continues until SimpleFrom is fully simplified
 * @note Uses SimplifyType::GetSimplifiedValue() to unwrap values
 *
 * Example:
 * @code
 * // Suppose From = const Base, SimpleFrom = Base
 * // IsaImplWrap<Derived, const Base, Base>::Check(val)
 * //   → Simplifies const Base to Base
 * //   → Recursively calls IsaImplWrap<Derived, Base, Base>::Check()
 * //   → Reaches specialization, calls IsaImplConst<Derived, Base>::Check()
 * @endcode
 *
 * @see IsaImplWrap<To, FromTy, FromTy> for the base case (fully simplified)
 * @see SimplifyType for the type simplification mechanism
 */
template <typename To, typename From, typename SimpleFrom>
struct IsaImplWrap {
    static bool Check(const From &Val) {
        CRLB_ZONE_SCOPED;
        return IsaImplWrap<To, SimpleFrom, typename SimplifyType<SimpleFrom>::SimpleType>::Check(
            SimplifyType<const From>::GetSimplifiedValue(Val));
    }
};

/**
 * @ingroup casting_detail
 * @brief Specialization for fully simplified types (base case).
 *
 * When From and SimpleFrom are the same type, it means SimplifyType has finished
 * unwrapping and we've reached a "simple" type. At this point, delegate to
 * IsaImplConst for the actual type check.
 *
 * @tparam To The target type to check for
 * @tparam FromTy The fully simplified source type
 *
 * ## Static Methods
 *
 * - `Check(const FromTy &Val)`: Delegates to IsaImplConst for type checking
 *
 * @note Base case of the recursive simplification
 * @note FromTy should be a "simple" type with no further unwrapping needed
 *
 * Example:
 * @code
 * // When recursion reaches a simple type like Base or Base*
 * IsaImplWrap<Derived, Base, Base>::Check(base_ref)
 *   → IsaImplConst<Derived, Base>::Check(base_ref)
 *   → IsaImpl<Derived, Base>::Check(base_ref)
 *   → Derived::classof(&base_ref)
 * @endcode
 *
 * @see IsaImplConst for const and pointer handling
 */
template <typename To, typename FromTy>
struct IsaImplWrap<To, FromTy, FromTy> {
    static bool Check(const FromTy &Val) {
        CRLB_ZONE_SCOPED;
        return IsaImplConst<To, FromTy>::Check(Val);
    }
};

}  // namespace corelib::detail

#endif  // LOGENIUM_CORELIB_DETAIL_CASTING_ISA_HXX
