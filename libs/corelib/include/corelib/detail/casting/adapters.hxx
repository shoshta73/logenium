// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_DETAIL_CASTING_ADAPTERS_HXX
#define LOGENIUM_CORELIB_DETAIL_CASTING_ADAPTERS_HXX

#include <memory>
#include <optional>
#include <type_traits>

#include <corelib/detail/casting/forwarding.hxx>
#include <corelib/detail/casting/traits.hxx>
#include <corelib/internal/tracing.hxx>

namespace corelib::detail {

// Forward declarations to avoid circular dependency with cast.hxx
template <typename To, typename From, typename Enable = void>
struct CastIsPossible;

template <class To, class From>
struct CastRetty;

// =============================================================================
// Section 1: Unique Pointer Casting
// =============================================================================

/**
 * @ingroup casting_detail
 * @brief Casting adapter for std::unique_ptr with ownership transfer.
 *
 * UniquePtrCast enables casting of std::unique_ptr instances while preserving
 * unique ownership semantics. When a cast succeeds, ownership is transferred
 * from the source unique_ptr to the result. When a cast fails (in DoCastIfPossible),
 * the source unique_ptr retains ownership.
 *
 * The implementation:
 * 1. Checks if the cast is possible via CastIsPossible (inherited)
 * 2. If possible, releases the source pointer and wraps it in a new unique_ptr
 * 3. If not possible, returns nullptr unique_ptr (source unchanged)
 *
 * @tparam To The target type (for unique_ptr<To>)
 * @tparam From The source type (from unique_ptr<From>)
 * @tparam Derived Optional derived class for CRTP customization
 *
 * ## Member Types
 *
 * - `Self`: CRTP self type (Derived or UniquePtrCast<To, From>)
 * - `CastResultType`: The result type (std::unique_ptr<To>)
 *
 * ## Static Methods
 *
 * - `DoCast(std::unique_ptr<From> &&f)`: Transfers ownership, returns unique_ptr<To>
 * - `CastFailed()`: Returns empty unique_ptr<To>
 * - `DoCastIfPossible(std::unique_ptr<From> &f)`: Checks possibility first,
 *   transfers ownership only if cast succeeds
 *
 * @note Inherits IsPossible from CastIsPossible<To, From*>
 * @note DoCast unconditionally transfers ownership (caller must check first)
 * @note DoCastIfPossible only transfers ownership if cast succeeds
 * @note Uses CRTP pattern via SelfType for customization
 *
 * Example:
 * @code
 * std::unique_ptr<Base> base_ptr = std::make_unique<Derived>();
 *
 * // Check if cast is possible
 * if (UniquePtrCast<Derived, Base>::IsPossible(base_ptr.get())) {
 *     auto derived_ptr = UniquePtrCast<Derived, Base>::DoCast(std::move(base_ptr));
 *     // base_ptr is now empty, derived_ptr owns the object
 * }
 *
 * // Or use DoCastIfPossible for conditional casting
 * auto result = UniquePtrCast<Derived, Base>::DoCastIfPossible(base_ptr);
 * // If successful: result owns the object, base_ptr is empty
 * // If failed: result is nullptr, base_ptr still owns the object
 * @endcode
 *
 * @see unique_dyn_cast() for the public API
 * @see SelfType for CRTP pattern
 */
template <typename To, typename From, typename Derived = void>
struct UniquePtrCast : CastIsPossible<To, From *> {
    using Self = SelfType<Derived, UniquePtrCast<To, From>>;
    using CastResultType = std::unique_ptr<std::remove_reference_t<typename CastRetty<To, From>::ret_type>>;

    static inline CastResultType DoCast(std::unique_ptr<From> &&f) {
        CRLB_ZONE_SCOPED;
        return CastResultType((typename CastResultType::element_type *)f.release());
    }

    static inline CastResultType CastFailed() {
        CRLB_ZONE_SCOPED;
        return CastResultType(nullptr);
    }

    static inline CastResultType DoCastIfPossible(std::unique_ptr<From> &f) {
        CRLB_ZONE_SCOPED;
        if (!Self::IsPossible(f.get())) return CastFailed();
        return DoCast(std::move(f));
    }
};

// =============================================================================
// Section 2: Optional Casting
// =============================================================================

/**
 * @ingroup casting_detail
 * @brief Casting adapter for optional-returning casts.
 *
 * OptionalValueCast enables casts that return std::optional<To>, where a
 * successful cast produces an optional with a value, and a failed cast produces
 * an empty optional. This is useful for value types and copy-constructible casts.
 *
 * The implementation:
 * 1. Inherits IsPossible from CastIsPossible<To, From>
 * 2. Inherits DoCastIfPossible from DefaultDoCastIfPossible (check then cast)
 * 3. Provides DoCast that constructs To from From
 * 4. Returns empty optional on failure
 *
 * @tparam To The target value type
 * @tparam From The source type
 * @tparam Derived Optional derived class for CRTP customization
 *
 * ## Static Methods
 *
 * - `CastFailed()`: Returns empty std::optional<To>
 * - `DoCast(const From &f)`: Constructs To from f, returns std::optional<To>
 * - `DoCastIfPossible(const From &f)`: Inherited - checks then casts
 *
 * @note Inherits IsPossible from CastIsPossible<To, From>
 * @note Inherits DoCastIfPossible from DefaultDoCastIfPossible
 * @note Requires To to be constructible from From
 * @note Uses CRTP pattern via SelfType for customization
 *
 * Example:
 * @code
 * // Suppose we have types where Derived constructs from Base
 * Base b = ...;
 *
 * if (OptionalValueCast<Derived, Base>::IsPossible(b)) {
 *     auto opt = OptionalValueCast<Derived, Base>::DoCast(b);
 *     // opt contains Derived(b)
 * }
 *
 * // Or use DoCastIfPossible
 * auto result = OptionalValueCast<Derived, Base>::DoCastIfPossible(b);
 * // result is either std::optional<Derived>{Derived(b)} or std::nullopt
 * @endcode
 *
 * @see DefaultDoCastIfPossible for conditional casting logic
 * @see SelfType for CRTP pattern
 */
template <typename To, typename From, typename Derived = void>
struct OptionalValueCast
    : public CastIsPossible<To, From>,
      public DefaultDoCastIfPossible<std::optional<To>, From, SelfType<Derived, OptionalValueCast<To, From>>> {
    static inline std::optional<To> CastFailed() {
        CRLB_ZONE_SCOPED;
        return std::optional<To>{};
    }

    static inline std::optional<To> DoCast(const From &f) {
        CRLB_ZONE_SCOPED;
        return To(f);
    }
};

// =============================================================================
// Section 3: Pointer-to-Value Casting
// =============================================================================

/**
 * @ingroup casting_detail
 * @brief Casting adapter for converting pointers to values.
 *
 * ValueFromPointerCast enables casting from pointer types (From*) to value
 * types (To), where To is constructible from From*. This is useful for wrapper
 * types or smart pointer-like classes that can be constructed from raw pointers.
 *
 * The implementation:
 * 1. Inherits IsPossible from CastIsPossible<To, From*> (pointer type checking)
 * 2. Inherits CastFailed from NullableValueCastFailed<To> (returns To(nullptr))
 * 3. Inherits DoCastIfPossible from DefaultDoCastIfPossible (check then cast)
 * 4. Provides DoCast that constructs To from From*
 *
 * @tparam To The target value type (must be constructible from From*)
 * @tparam From The pointed-to type
 * @tparam Derived Optional derived class for CRTP customization
 *
 * ## Static Methods
 *
 * - `DoCast(From *f)`: Constructs To from f, returns To
 * - `CastFailed()`: Inherited - returns To(nullptr)
 * - `DoCastIfPossible(From *f)`: Inherited - checks then casts
 *
 * @note Inherits IsPossible from CastIsPossible<To, From*>
 * @note Inherits CastFailed from NullableValueCastFailed<To>
 * @note Inherits DoCastIfPossible from DefaultDoCastIfPossible
 * @note Requires To to be constructible from From*
 * @note Used for pointer-to-wrapper conversions
 * @note Uses CRTP pattern via SelfType for customization
 *
 * Example:
 * @code
 * // Suppose To is a wrapper class constructible from pointers
 * struct Wrapper {
 *     Base *ptr;
 *     Wrapper(Base *p) : ptr(p) {}
 *     Wrapper(std::nullptr_t) : ptr(nullptr) {}
 * };
 *
 * Derived *d = new Derived();
 *
 * if (ValueFromPointerCast<Wrapper, Base>::IsPossible(d)) {
 *     Wrapper w = ValueFromPointerCast<Wrapper, Base>::DoCast(d);
 *     // w wraps the pointer
 * }
 *
 * // Or use DoCastIfPossible
 * auto result = ValueFromPointerCast<Wrapper, Base>::DoCastIfPossible(d);
 * // result is either Wrapper(d) or Wrapper(nullptr)
 * @endcode
 *
 * @see NullableValueCastFailed for failure handling
 * @see DefaultDoCastIfPossible for conditional casting logic
 * @see SelfType for CRTP pattern
 */
template <typename To, typename From, typename Derived = void>
struct ValueFromPointerCast
    : public CastIsPossible<To, From *>,
      public NullableValueCastFailed<To>,
      public DefaultDoCastIfPossible<To, From *, SelfType<Derived, ValueFromPointerCast<To, From>>> {
    static inline To DoCast(From *f) {
        CRLB_ZONE_SCOPED;
        return To(f);
    }
};

}  // namespace corelib::detail

#endif  // LOGENIUM_CORELIB_DETAIL_CASTING_ADAPTERS_HXX
