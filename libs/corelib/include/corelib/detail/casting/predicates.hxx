// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_DETAIL_CASTING_PREDICATES_HXX
#define LOGENIUM_CORELIB_DETAIL_CASTING_PREDICATES_HXX

#include <corelib/casting/cast.hxx>
#include <corelib/casting/dyn_cast.hxx>
#include <corelib/casting/isa.hxx>

namespace corelib::detail {

/**
 * @ingroup casting_detail
 * @brief Predicate functor for variadic type checking.
 *
 * IsaCheckPredicate wraps the isa<Types...>() function in a functor suitable
 * for use with standard algorithms. It checks if a value is an instance of
 * any of the specified types.
 *
 * @tparam Types The target types to check for (variadic)
 *
 * @note Used internally by public IsaPred variable template
 *
 * Example:
 * @code
 * std::vector<Base*> ptrs = ...;
 * IsaCheckPredicate<Derived> pred;
 *
 * auto it = std::find_if(ptrs.begin(), ptrs.end(), pred);
 * // Finds first element that is a Derived*
 * @endcode
 *
 * @see isa() for the underlying type-checking function
 * @see IsaPred for the public variable template interface
 */
template <typename... Types>
struct IsaCheckPredicate {
    template <typename T>
    bool operator()(const T &Val) const {
        return isa<Types...>(Val);
    }
};

/**
 * @ingroup casting_detail
 * @brief Predicate functor for type checking with presence validation.
 *
 * IsaAndPresentCheckPredicate wraps isa_and_present<Types...>() in a functor.
 * It checks if a value is present (not null/empty) AND is an instance of any
 * of the specified types.
 *
 * @tparam Types The target types to check for (variadic)
 *
 * @note Used internally by public IsaAndPresentPred variable template
 *
 * Example:
 * @code
 * std::vector<Base*> ptrs = ...;  // May contain nullptr
 * IsaAndPresentCheckPredicate<Derived> pred;
 *
 * auto it = std::find_if(ptrs.begin(), ptrs.end(), pred);
 * // Finds first non-null element that is a Derived*
 * @endcode
 *
 * @see isa_and_present() for the underlying function
 * @see IsaAndPresentPred for the public variable template interface
 */
template <typename... Types>
struct IsaAndPresentCheckPredicate {
    template <typename T>
    bool operator()(const T &Val) const {
        return isa_and_present<Types...>(Val);
    }
};

/**
 * @ingroup casting_detail
 * @brief Functor for static_cast operations.
 *
 * StaticCastFunc wraps static_cast<U> in a functor for use with algorithms.
 * Uses perfect forwarding to preserve value categories.
 *
 * @tparam U The target type for the cast
 *
 * @note Used internally by public StaticCastTo variable template
 *
 * Example:
 * @code
 * std::vector<double> doubles = {1.5, 2.5, 3.5};
 * std::vector<int> ints;
 *
 * std::transform(doubles.begin(), doubles.end(),
 *                std::back_inserter(ints),
 *                StaticCastFunc<int>{});
 * // ints = {1, 2, 3}
 * @endcode
 *
 * @see StaticCastTo for the public variable template interface
 */
template <typename U>
struct StaticCastFunc {
    template <typename T>
    decltype(auto) operator()(T &&Val) const {
        return static_cast<U>(Val);
    }
};

/**
 * @ingroup casting_detail
 * @brief Functor for dyn_cast operations.
 *
 * DynCastFunc wraps dyn_cast<U> in a functor for use with algorithms.
 * Uses perfect forwarding to preserve value categories.
 *
 * @tparam U The target type for the cast
 *
 * @note Used internally by public DynCastTo variable template
 *
 * Example:
 * @code
 * std::vector<Base*> bases = ...;
 * std::vector<Derived*> deriveds;
 *
 * std::transform(bases.begin(), bases.end(),
 *                std::back_inserter(deriveds),
 *                DynCastFunc<Derived*>{});
 * // Filters and casts to Derived*, nullptrs where cast fails
 * @endcode
 *
 * @see dyn_cast() for the underlying function
 * @see DynCastTo for the public variable template interface
 */
template <typename U>
struct DynCastFunc {
    template <typename T>
    decltype(auto) operator()(T &&Val) const {
        return dyn_cast<U>(Val);
    }
};

/**
 * @ingroup casting_detail
 * @brief Functor for cast operations.
 *
 * CastFunc wraps cast<U> in a functor for use with algorithms.
 * Uses perfect forwarding to preserve value categories.
 *
 * @tparam U The target type for the cast
 *
 * @note Used internally by public CastTo variable template
 *
 * Example:
 * @code
 * std::vector<Base*> bases = ...;
 * std::vector<Derived*> deriveds;
 *
 * std::transform(bases.begin(), bases.end(),
 *                std::back_inserter(deriveds),
 *                CastFunc<Derived*>{});
 * // Casts with assertion on type mismatch
 * @endcode
 *
 * @see cast() for the underlying function
 * @see CastTo for the public variable template interface
 */
template <typename U>
struct CastFunc {
    template <typename T>
    decltype(auto) operator()(T &&Val) const {
        return cast<U>(Val);
    }
};

/**
 * @ingroup casting_detail
 * @brief Functor for conditional cast operations.
 *
 * CastIfPresentFunc wraps cast_if_present<U> in a functor for use with algorithms.
 * Uses perfect forwarding to preserve value categories.
 *
 * @tparam U The target type for the cast
 *
 * @note Used internally by public CastIfPresentTo variable template
 *
 * Example:
 * @code
 * std::vector<std::optional<Base>> opts = ...;
 * std::vector<Derived> deriveds;
 *
 * std::transform(opts.begin(), opts.end(),
 *                std::back_inserter(deriveds),
 *                CastIfPresentFunc<Derived>{});
 * // Casts only when optional has value
 * @endcode
 *
 * @see cast_if_present() for the underlying function
 * @see CastIfPresentTo for the public variable template interface
 */
template <typename U>
struct CastIfPresentFunc {
    template <typename T>
    decltype(auto) operator()(T &&Val) const {
        return cast_if_present<U>(Val);
    }
};

/**
 * @ingroup casting_detail
 * @brief Functor for conditional dynamic cast operations.
 *
 * DynCastIfPresentFunc wraps dyn_cast_if_present<U> in a functor for use with algorithms.
 * Uses perfect forwarding to preserve value categories.
 *
 * @tparam U The target type for the cast
 *
 * @note Used internally by public DynCastIfPresentTo variable template
 *
 * Example:
 * @code
 * std::vector<std::optional<Base*>> opts = ...;
 * std::vector<Derived*> deriveds;
 *
 * std::transform(opts.begin(), opts.end(),
 *                std::back_inserter(deriveds),
 *                DynCastIfPresentFunc<Derived*>{});
 * // Casts only when optional has value, returns nullptr on failure
 * @endcode
 *
 * @see dyn_cast_if_present() for the underlying function
 * @see DynCastIfPresentTo for the public variable template interface
 */
template <typename U>
struct DynCastIfPresentFunc {
    template <typename T>
    decltype(auto) operator()(T &&Val) const {
        return dyn_cast_if_present<U>(Val);
    }
};

}  // namespace corelib::detail

#endif  // LOGENIUM_CORELIB_DETAIL_CASTING_PREDICATES_HXX
