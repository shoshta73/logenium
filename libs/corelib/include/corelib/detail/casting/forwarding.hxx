// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_DETAIL_CASTING_FORWARDING_HXX
#define LOGENIUM_CORELIB_DETAIL_CASTING_FORWARDING_HXX

#include <type_traits>

namespace corelib::detail {

// =============================================================================
// Section 1: Default Cast Failed Implementations
// =============================================================================

/**
 * @ingroup casting_detail
 * @brief Provides default CastFailed() implementation for nullable types.
 *
 * This helper struct provides a static CastFailed() method that returns
 * To(nullptr), suitable for nullable types (pointers, smart pointers, etc.).
 * It's used as a base class or building block for cast operations that need
 * to return a null value when a cast fails.
 *
 * @tparam To The target nullable type (must be constructible from nullptr)
 *
 * ## Static Methods
 *
 * - `CastFailed()`: Returns To(nullptr) to indicate cast failure
 *
 * @note Only suitable for nullable types (pointers, std::unique_ptr, etc.)
 * @note Used internally by dyn_cast_or_null() and similar operations
 *
 * Example:
 * @code
 * // For pointer types
 * int *failed_ptr = NullableValueCastFailed<int*>::CastFailed();
 * // failed_ptr is nullptr
 *
 * // For smart pointers
 * auto failed_unique = NullableValueCastFailed<std::unique_ptr<int>>::CastFailed();
 * // failed_unique is an empty unique_ptr
 * @endcode
 *
 * @see DefaultDoCastIfPossible for conditional casting logic
 */
template <typename To>
struct NullableValueCastFailed {
    static To CastFailed() { return To(nullptr); }
};

/**
 * @ingroup casting_detail
 * @brief CRTP base providing default DoCastIfPossible() implementation.
 *
 * This template implements the conditional casting pattern using CRTP
 * (Curiously Recurring Template Pattern). It provides a DoCastIfPossible()
 * method that checks if a cast is possible before attempting it, returning
 * a failure value if the cast would fail.
 *
 * The derived class must provide:
 * - `IsPossible(From f)`: Returns true if the cast can succeed
 * - `DoCast(From f)`: Performs the actual cast (assumes success)
 * - `CastFailed()`: Returns the value to use when cast fails
 *
 * This pattern is used throughout the casting infrastructure to implement
 * dyn_cast() and similar operations that need to check type compatibility
 * before casting.
 *
 * @tparam To The target type of the cast
 * @tparam From The source type of the cast
 * @tparam Derived The derived class implementing the actual cast logic (CRTP)
 *
 * ## Static Methods
 *
 * - `DoCastIfPossible(From f)`: Conditionally casts f to To, calling
 *   Derived::IsPossible() to check, Derived::DoCast() to perform, and
 *   Derived::CastFailed() if the cast would fail
 *
 * @note Uses CRTP pattern - Derived must inherit from this class
 * @note Provides the implementation for dyn_cast() operations
 *
 * Example:
 * @code
 * template <typename To, typename From>
 * struct MyCastImpl : DefaultDoCastIfPossible<To, From, MyCastImpl<To, From>> {
 *     static bool IsPossible(From f) {
 *         // Check if cast is possible
 *         return; // type check logic
 *     }
 *
 *     static To DoCast(From f) {
 *         // Perform the cast
 *         return static_cast<To>(f);
 *     }
 *
 *     static To CastFailed() {
 *         // Return failure value
 *         return To(nullptr);
 *     }
 * };
 *
 * // Now MyCastImpl has DoCastIfPossible() automatically
 * auto result = MyCastImpl<Derived*, Base*>::DoCastIfPossible(base_ptr);
 * @endcode
 *
 * @see NullableValueCastFailed for a common CastFailed() implementation
 * @see SelfType for CRTP pattern support
 */
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

/**
 * @ingroup casting_detail
 * @brief Forwards reference-based casts to pointer-based cast implementations.
 *
 * This forwarding strategy enables reference casts to be implemented in terms
 * of pointer casts. It takes a reference, converts it to a pointer, forwards
 * to the pointer-based cast implementation, then dereferences the result.
 *
 * This is a key optimization in the casting infrastructure - reference casts
 * don't need separate implementations, they can reuse the pointer cast logic.
 *
 * @tparam To The target type (typically a reference type)
 * @tparam From The source reference type
 * @tparam ForwardTo The pointer-based cast implementation to forward to
 *
 * ## Static Methods
 *
 * - `IsPossible(const From &f)`: Checks if cast is possible by taking address
 *   of f and calling ForwardTo::IsPossible(&f)
 * - `DoCast(const From &f)`: Performs cast by taking address of f, calling
 *   ForwardTo::DoCast(&f), then dereferencing the result
 *
 * @note Assumes ForwardTo operates on pointer types
 * @note Result is dereferenced, converting pointer back to reference
 * @note Uses decltype(auto) to preserve reference types and const-correctness
 *
 * Example:
 * @code
 * // Suppose we have a pointer cast implementation
 * struct MyPointerCast {
 *     static bool IsPossible(Base *ptr) { return isa<Derived>(ptr); }
 *     static Derived *DoCast(Base *ptr) { return static_cast<Derived*>(ptr); }
 * };
 *
 * // ForwardToPointerCast enables reference casts
 * using MyRefCast = ForwardToPointerCast<Derived&, Base&, MyPointerCast>;
 *
 * Base &ref = ...;
 * if (MyRefCast::IsPossible(ref)) {
 *     Derived &d = MyRefCast::DoCast(ref);  // Forwarded to pointer cast, then dereferenced
 * }
 * @endcode
 *
 * @see ConstStrippingForwardingCast for const-aware forwarding
 */
template <typename To, typename From, typename ForwardTo>
struct ForwardToPointerCast {
    static inline bool IsPossible(const From &f) { return ForwardTo::IsPossible(&f); }

    static inline decltype(auto) DoCast(const From &f) { return *ForwardTo::DoCast(&f); }
};

/**
 * @ingroup casting_detail
 * @brief Forwards const-qualified casts to non-const cast implementations.
 *
 * This forwarding strategy handles const-correctness by stripping const
 * qualifiers from the source type, forwarding to a non-const cast implementation,
 * then allowing the result to inherit appropriate const-ness. This enables
 * const-qualified casts to reuse non-const implementations.
 *
 * The strategy works for both pointers and references:
 * - For `const T*`: strips to `T*`, forwards, result gets const back via return type
 * - For `const T&`: strips to `T&`, forwards, result gets const back via return type
 *
 * This is essential for implementing the const-correctness guarantees of the
 * casting system - casting from `const Base*` should produce `const Derived*`,
 * but the implementation can reuse the non-const logic.
 *
 * @tparam To The target type
 * @tparam From The source type (may be const-qualified pointer or reference)
 * @tparam ForwardTo The non-const cast implementation to forward to
 *
 * ## Member Types
 *
 * - `DecayedFrom`: From with all cv-qualifiers and pointer removed
 * - `NonConstFrom`: From reconstructed without const (T* for pointers, T& for refs)
 *
 * ## Static Methods
 *
 * - `IsPossible(const From &f)`: Strips const from f and calls ForwardTo::IsPossible()
 * - `CastFailed()`: Forwards to ForwardTo::CastFailed()
 * - `DoCast(const From &f)`: Strips const from f and calls ForwardTo::DoCast()
 * - `DoCastIfPossible(const From &f)`: Strips const from f and calls
 *   ForwardTo::DoCastIfPossible()
 *
 * @note Uses const_cast to safely remove const before forwarding
 * @note Preserves pointer vs reference distinction
 * @note Return type preserves const-correctness automatically
 *
 * Example:
 * @code
 * // Suppose we have a non-const cast implementation
 * struct NonConstCastImpl {
 *     static bool IsPossible(Base *ptr) { return isa<Derived>(ptr); }
 *     static Derived *DoCast(Base *ptr) { return static_cast<Derived*>(ptr); }
 *     static Derived *CastFailed() { return nullptr; }
 * };
 *
 * // ConstStrippingForwardingCast enables const casts
 * using ConstCastImpl = ConstStrippingForwardingCast<
 *     const Derived*, const Base*, NonConstCastImpl>;
 *
 * const Base *const_base = ...;
 * if (ConstCastImpl::IsPossible(const_base)) {
 *     // Const is stripped, forwarded, then preserved in return type
 *     const Derived *const_derived = ConstCastImpl::DoCast(const_base);
 * }
 * @endcode
 *
 * @see ForwardToPointerCast for reference-to-pointer forwarding
 */
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
