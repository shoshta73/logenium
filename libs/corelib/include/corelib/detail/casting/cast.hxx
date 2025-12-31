// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_DETAIL_CASTING_CAST_HXX
#define LOGENIUM_CORELIB_DETAIL_CASTING_CAST_HXX

#include <memory>
#include <optional>
#include <type_traits>

#include <debug/assert.hxx>

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

/**
 * @ingroup casting_detail
 * @brief Base template for calculating cast return types.
 *
 * CastRettyImpl determines the appropriate return type for a cast based on the
 * source type's qualifiers (const, pointer, reference). It preserves const-correctness
 * and reference/pointer semantics in the result type.
 *
 * The base template handles non-const reference types, returning To&.
 * Specializations handle:
 * - const-qualified types (preserves const)
 * - Pointer types (returns pointers, preserves const)
 * - std::unique_ptr (returns std::unique_ptr with correct const)
 *
 * @tparam To The target type
 * @tparam From The source type (may include qualifiers)
 *
 * ## Member Types
 *
 * - `ret_type`: The calculated return type for the cast
 *
 * @note Base template: From → To&
 * @note Used internally by CastRetty via CastRettyWrap
 *
 * @see CastRetty for the main return type calculator
 * @see CastRettyWrap for type simplification integration
 */
template <class To, class From>
struct CastRettyImpl {
    using ret_type = To &;
};

/**
 * @ingroup casting_detail
 * @brief Specialization for const-qualified types.
 *
 * Preserves const qualification in the return type.
 *
 * @tparam To The target type
 * @tparam From The const-qualified source type
 *
 * @note const From → const To&
 */
template <class To, class From>
struct CastRettyImpl<To, const From> {
    using ret_type = const To &;
};

/**
 * @ingroup casting_detail
 * @brief Specialization for pointer types.
 *
 * Returns a pointer to the target type.
 *
 * @tparam To The target type
 * @tparam From The pointed-to type
 *
 * @note From* → To*
 */
template <class To, class From>
struct CastRettyImpl<To, From *> {
    using ret_type = To *;
};

/**
 * @ingroup casting_detail
 * @brief Specialization for pointer-to-const types.
 *
 * Returns a pointer to const target type, preserving const on the pointee.
 *
 * @tparam To The target type
 * @tparam From The const-qualified pointed-to type
 *
 * @note const From* → const To*
 */
template <class To, class From>
struct CastRettyImpl<To, const From *> {
    using ret_type = const To *;
};

/**
 * @ingroup casting_detail
 * @brief Specialization for const-pointer-to-const types.
 *
 * Returns a pointer to const target type (top-level const on pointer is dropped).
 *
 * @tparam To The target type
 * @tparam From The const-qualified pointed-to type
 *
 * @note const From* const → const To*
 */
template <class To, class From>
struct CastRettyImpl<To, const From *const> {
    using ret_type = const To *;
};

/**
 * @ingroup casting_detail
 * @brief Specialization for std::unique_ptr types.
 *
 * Returns a std::unique_ptr to the target type, preserving const-correctness
 * from the pointee. Uses CastRettyImpl<To, From*> to calculate the pointer
 * type, then wraps it in unique_ptr.
 *
 * @tparam To The target type
 * @tparam From The type contained in the unique_ptr
 *
 * @note std::unique_ptr<From> → std::unique_ptr<To>
 * @note std::unique_ptr<const From> → std::unique_ptr<const To>
 */
template <class To, class From>
struct CastRettyImpl<To, std::unique_ptr<From>> {
  private:
    using PointerType = typename CastRettyImpl<To, From *>::ret_type;
    using ResultType = std::remove_pointer_t<PointerType>;

  public:
    using ret_type = std::unique_ptr<ResultType>;
};

/**
 * @ingroup casting_detail
 * @brief Recursive wrapper for return type calculation with type simplification.
 *
 * CastRettyWrap integrates SimplifyType's recursive unwrapping with return type
 * calculation. It recursively simplifies the From type until reaching a "simple"
 * type, then delegates to CastRettyImpl for the actual return type calculation.
 *
 * The base template provides the recursive case, continuing to simplify.
 * The specialization (when From == SimpleFrom) is the base case, delegating
 * to CastRettyImpl.
 *
 * @tparam To The target type
 * @tparam From The original source type (potentially with qualifiers)
 * @tparam SimpleFrom The simplified version of From
 *
 * ## Member Types
 *
 * - `ret_type`: The calculated return type after simplification
 *
 * @note Recursive template - continues until From == SimpleFrom
 *
 * @see CastRettyImpl for the base return type calculation
 * @see CastRetty for the main entry point
 */
template <class To, class From, class SimpleFrom>
struct CastRettyWrap {
    using ret_type = typename CastRetty<To, SimpleFrom>::ret_type;
};

/**
 * @ingroup casting_detail
 * @brief Specialization for fully simplified types (base case).
 *
 * When From and SimpleFrom are the same, simplification is complete.
 * Delegate to CastRettyImpl for the actual return type calculation.
 *
 * @tparam To The target type
 * @tparam FromTy The fully simplified source type
 *
 * ## Member Types
 *
 * - `ret_type`: Delegates to CastRettyImpl<To, FromTy>::ret_type
 */
template <class To, class FromTy>
struct CastRettyWrap<To, FromTy, FromTy> {
    using ret_type = typename CastRettyImpl<To, FromTy>::ret_type;
};

/**
 * @ingroup casting_detail
 * @brief Main return type calculator with type simplification.
 *
 * CastRetty is the primary interface for determining the return type of a cast
 * operation. It integrates type simplification (via SimplifyType) with return
 * type calculation (via CastRettyImpl).
 *
 * The process:
 * 1. Simplify From using SimplifyType
 * 2. Recursively apply simplification via CastRettyWrap
 * 3. Once simplified, calculate return type via CastRettyImpl
 *
 * @tparam To The target type
 * @tparam From The source type (may have qualifiers, wrappers, etc.)
 *
 * ## Member Types
 *
 * - `ret_type`: The calculated return type for cast<To>(From)
 *
 * @note Used throughout the casting infrastructure to determine return types
 * @note Preserves const-correctness and reference/pointer semantics
 *
 * Example:
 * @code
 * // Non-const reference
 * static_assert(std::is_same_v<CastRetty<Derived, Base>::ret_type, Derived&>);
 *
 * // Const reference
 * static_assert(std::is_same_v<CastRetty<Derived, const Base>::ret_type, const Derived&>);
 *
 * // Pointer
 * static_assert(std::is_same_v<CastRetty<Derived, Base*>::ret_type, Derived*>);
 *
 * // Const pointer
 * static_assert(std::is_same_v<CastRetty<Derived, const Base*>::ret_type, const Derived*>);
 *
 * // Unique ptr
 * static_assert(std::is_same_v<
 *     CastRetty<Derived, std::unique_ptr<Base>>::ret_type,
 *     std::unique_ptr<Derived>>);
 * @endcode
 *
 * @see CastRettyImpl for the core return type calculations
 * @see CastRettyWrap for type simplification integration
 */
template <class To, class From>
struct CastRetty {
    using ret_type = typename CastRettyWrap<To, From, typename SimplifyType<From>::SimpleType>::ret_type;
};

// =============================================================================
// Section 2: Cast Possibility Checks
// =============================================================================

/**
 * @ingroup casting_detail
 * @brief Base template for checking if a cast is possible.
 *
 * CastIsPossible determines whether a cast from From to To can succeed by
 * delegating to the IsaImplWrap infrastructure. This integrates type
 * simplification with the classof()-based type checking.
 *
 * The base template handles general types by simplifying and checking.
 * Specializations handle:
 * - std::optional (unwraps and checks the contained value)
 * - Trivial upcasts (From derives from To, always possible)
 *
 * @tparam To The target type
 * @tparam From The source type
 * @tparam Enable SFINAE enabler for specializations
 *
 * ## Static Methods
 *
 * - `IsPossible(const From &f)`: Returns true if cast<To>(f) would succeed
 *
 * @note Delegates to IsaImplWrap for the actual type checking
 * @note Used by CastInfo and conditional casting operations
 *
 * @see IsaImplWrap for the type-checking infrastructure
 */
template <typename To, typename From, typename Enable>
struct CastIsPossible {
    static inline bool IsPossible(const From &f) {
        return detail::IsaImplWrap<To, const From, typename detail::SimplifyType<const From>::SimpleType>::Check(f);
    }
};

/**
 * @ingroup casting_detail
 * @brief Specialization for std::optional types.
 *
 * Checks if the cast is possible for the contained value. Asserts that the
 * optional has a value before checking.
 *
 * @tparam To The target type
 * @tparam From The type contained in the optional
 *
 * ## Static Methods
 *
 * - `IsPossible(const std::optional<From> &f)`: Unwraps f and checks if cast
 *   is possible (asserts f.has_value())
 *
 * @note Asserts the optional is not empty using debug::Assert
 */
template <typename To, typename From>
struct CastIsPossible<To, std::optional<From>> {
    static inline bool IsPossible(const std::optional<From> &f) {
        debug::Assert(f.has_value(),
                      "CastIsPossible<{}, std::optional<{}>>::IsPossible(const std::optional<{}>) called on a nullopt!",
                      type_name<To>(), type_name<From>(), type_name<From>());
        return detail::IsaImplWrap<To, const From, typename detail::SimplifyType<const From>::SimpleType>::Check(*f);
    }
};

/**
 * @ingroup casting_detail
 * @brief Specialization for trivial upcasts (From derives from To).
 *
 * When From is derived from To, the cast is always possible without checking.
 * This is an optimization for the trivial upcast case.
 *
 * @tparam To The base type
 * @tparam From The derived type
 *
 * ## Static Methods
 *
 * - `IsPossible(const From &)`: Always returns true
 *
 * @note Enabled via SFINAE when std::is_base_of_v<To, From> is true
 */
template <typename To, typename From>
struct CastIsPossible<To, From, std::enable_if_t<std::is_base_of_v<To, From>>> {
    static inline bool IsPossible(const From &) { return true; }
};

// =============================================================================
// Section 3: Cast Conversion
// =============================================================================

/**
 * @ingroup casting_detail
 * @brief Recursive cast conversion with type simplification.
 *
 * CastConvertVal performs the actual cast operation by:
 * 1. Recursively simplifying the From type using SimplifyType
 * 2. Once simplified, performing the cast via pointer/reference manipulation
 *
 * The base template provides the recursive case. Specializations handle:
 * - Fully simplified reference types (base case)
 * - Pointer types (simple pointer cast)
 *
 * @tparam To The target type
 * @tparam From The original source type (potentially with qualifiers)
 * @tparam SimpleFrom The simplified version of From
 *
 * ## Static Methods
 *
 * - `Convert(const From &Val)`: Recursively simplifies and casts Val to To
 *
 * @note Recursive template - continues until From == SimpleFrom
 * @note Uses pointer/reference casting for the actual conversion
 *
 * @see CastRetty for return type calculation
 * @see SimplifyType for type simplification
 */
template <class To, class From, class SimpleFrom>
struct CastConvertVal {
    static typename CastRetty<To, From>::ret_type Convert(const From &Val) {
        return CastConvertVal<To, SimpleFrom, typename SimplifyType<SimpleFrom>::SimpleType>::Convert(
            SimplifyType<From>::GetSimplifiedValue(const_cast<From &>(Val)));
    }
};

/**
 * @ingroup casting_detail
 * @brief Specialization for fully simplified reference types (base case).
 *
 * When From == SimpleFrom, simplification is complete. Perform the cast by
 * pointer manipulation: take address of Val, cast pointer, then dereference.
 *
 * @tparam To The target type
 * @tparam FromTy The fully simplified source type
 *
 * ## Static Methods
 *
 * - `Convert(const FromTy &Val)`: Casts &Val to target pointer type, dereferences
 *
 * @note Uses pointer casting with const_cast for the conversion
 * @note Result is dereferenced to return a reference
 */
template <class To, class FromTy>
struct CastConvertVal<To, FromTy, FromTy> {
    static typename CastRetty<To, FromTy>::ret_type Convert(const FromTy &Val) {
        return *(std::remove_reference_t<typename CastRetty<To, FromTy>::ret_type> *)&const_cast<FromTy &>(Val);
    }
};

/**
 * @ingroup casting_detail
 * @brief Specialization for pointer types.
 *
 * For pointer types, the cast is a simple pointer cast (no dereferencing needed).
 *
 * @tparam To The target type
 * @tparam FromTy The pointed-to type
 *
 * ## Static Methods
 *
 * - `Convert(const FromTy *Val)`: Casts Val to target pointer type
 *
 * @note Direct pointer cast, no pointer/reference manipulation needed
 */
template <class To, class FromTy>
struct CastConvertVal<To, FromTy *, FromTy *> {
    static typename CastRetty<To, FromTy *>::ret_type Convert(const FromTy *Val) {
        return (typename CastRetty<To, FromTy *>::ret_type) const_cast<FromTy *>(Val);
    }
};

// =============================================================================
// Section 4: Main CastInfo Orchestrator
// =============================================================================

/**
 * @ingroup casting_detail
 * @brief Main orchestrator for cast operations.
 *
 * CastInfo is the central template that coordinates all casting operations.
 * It integrates:
 * - Return type calculation (CastRetty)
 * - Type checking (CastIsPossible)
 * - Cast conversion (CastConvertVal)
 * - Conditional casting (DoCastIfPossible pattern)
 *
 * The base template handles "simple" types (those that don't need further
 * simplification). Specializations handle:
 * - Non-simple types (recursive simplification)
 * - std::unique_ptr (delegates to UniquePtrCast)
 * - std::optional (delegates to OptionalValueCast)
 *
 * @tparam To The target type
 * @tparam From The source type
 * @tparam Enable SFINAE enabler for specializations
 *
 * ## Member Types
 *
 * - `Self`: The CastInfo type itself
 * - `CastReturnType`: The return type for the cast (from CastRetty)
 *
 * ## Static Methods
 *
 * - `DoCast(const From &f)`: Performs the cast (assumes success)
 * - `CastFailed()`: Returns nullptr (for failed casts)
 * - `DoCastIfPossible(const From &f)`: Checks then casts
 * - `IsPossible(const From &f)`: Inherited from CastIsPossible
 *
 * @note Inherits IsPossible from CastIsPossible
 * @note Base template for simple types
 * @note Used by cast(), dyn_cast(), and related public APIs
 *
 * Example:
 * @code
 * Base *base = ...;
 *
 * // Check if cast is possible
 * if (CastInfo<Derived, Base*>::IsPossible(base)) {
 *     // Perform the cast
 *     Derived *derived = CastInfo<Derived, Base*>::DoCast(base);
 * }
 *
 * // Or use conditional casting
 * Derived *result = CastInfo<Derived, Base*>::DoCastIfPossible(base);
 * // result is nullptr if cast failed
 * @endcode
 *
 * @see CastRetty for return type calculation
 * @see CastIsPossible for possibility checking
 * @see CastConvertVal for conversion
 */
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

/**
 * @ingroup casting_detail
 * @brief Specialization for non-simple types requiring simplification.
 *
 * When From is not a "simple" type (IsSimpleType<From>::value is false),
 * this specialization recursively simplifies the type and forwards all
 * operations to the simplified CastInfo.
 *
 * @tparam To The target type
 * @tparam From The non-simple source type
 *
 * ## Member Types
 *
 * - `Self`: The CastInfo type itself
 * - `SimpleFrom`: The simplified version of From
 * - `SimplifiedSelf`: CastInfo<To, SimpleFrom>
 *
 * ## Static Methods
 *
 * - `IsPossible(From &f)`: Simplifies f and forwards to SimplifiedSelf
 * - `DoCast(From &f)`: Simplifies f and forwards to SimplifiedSelf
 * - `CastFailed()`: Forwards to SimplifiedSelf
 * - `DoCastIfPossible(From &f)`: Simplifies f and forwards to SimplifiedSelf
 *
 * @note Enabled via SFINAE when !IsSimpleType<From>::value
 * @note All operations delegate to CastInfo<To, SimpleFrom>
 */
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

/**
 * @ingroup casting_detail
 * @brief Specialization for std::unique_ptr types.
 *
 * Delegates all operations to UniquePtrCast for ownership transfer semantics.
 *
 * @tparam To The target type
 * @tparam From The type contained in the unique_ptr
 *
 * @see UniquePtrCast for implementation details
 */
template <typename To, typename From>
struct CastInfo<To, std::unique_ptr<From>> : public UniquePtrCast<To, From> {};

/**
 * @ingroup casting_detail
 * @brief Specialization for std::optional types.
 *
 * Delegates all operations to OptionalValueCast for optional semantics.
 *
 * @tparam To The target type
 * @tparam From The type contained in the optional
 *
 * @see OptionalValueCast for implementation details
 */
template <typename To, typename From>
struct CastInfo<To, std::optional<From>> : OptionalValueCast<To, From> {};

}  // namespace corelib::detail

#endif  // LOGENIUM_CORELIB_DETAIL_CASTING_CAST_HXX
