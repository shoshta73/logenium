// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_CASTING_PREDICATES_HXX
#define LOGENIUM_CORELIB_CASTING_PREDICATES_HXX

#include <corelib/detail/casting/predicates.hxx>

namespace corelib {

/**
 * @ingroup casting
 * @brief Predicate functor for variadic type checking with algorithms.
 *
 * IsaPred is a constexpr variable template that provides a predicate functor
 * for use with standard algorithms (find_if, all_of, count_if, etc.) and ranges.
 * It wraps isa<Types...>() to check if a value is an instance of any of the
 * specified types.
 *
 * The predicate supports variadic type checking with OR semantics - returns true
 * if the value matches ANY of the specified types. This makes it ideal for
 * filtering, searching, and counting operations in containers.
 *
 * @tparam Types The target types to check for (variadic)
 *
 * @note Asserts if value is null (use IsaAndPresentPred for null-safe checking)
 * @note Requires Types to implement classof() for RTTI-free type checking
 * @note Works with pointers, references, and unique_ptr
 *
 * Example:
 * @code
 * std::vector<Base*> bases = {new Derived1(), new Derived2(), new Base()};
 *
 * // Find first Derived1
 * auto it = std::find_if(bases.begin(), bases.end(), IsaPred<Derived1>);
 *
 * // Count Derived1 or Derived2 instances
 * auto count = std::count_if(bases.begin(), bases.end(),
 *                             IsaPred<Derived1, Derived2>);
 *
 * // Check if all are Derived types
 * bool all_derived = std::all_of(bases.begin(), bases.end(),
 *                                 IsaPred<Derived1, Derived2>);
 *
 * // C++20 ranges
 * auto derived_view = bases | std::views::filter(IsaPred<Derived1>);
 *
 * // Lambda-style usage
 * auto pred = IsaPred<Derived>;
 * if (pred(base_ptr)) {
 *     // base_ptr is a Derived
 * }
 * @endcode
 *
 * @see isa() for the underlying type-checking function
 * @see IsaAndPresentPred for null-safe version
 */
template <typename... Types>
inline constexpr detail::IsaCheckPredicate<Types...> IsaPred{};

/**
 * @ingroup casting
 * @brief Null-safe predicate functor for type checking with algorithms.
 *
 * IsaAndPresentPred is a constexpr variable template that provides a null-safe
 * predicate functor for use with standard algorithms and ranges. It wraps
 * isa_and_present<Types...>() to check if a value is present (not null/empty)
 * AND is an instance of any of the specified types.
 *
 * Unlike IsaPred, this predicate never asserts - it safely handles null pointers
 * and empty optionals by returning false. This makes it ideal for filtering
 * containers that may contain null values.
 *
 * @tparam Types The target types to check for (variadic)
 *
 * @note No assertions - safe for null/empty values
 * @note Short-circuits: returns false immediately if value is not present
 * @note Supports variadic type checking with OR semantics
 *
 * Example:
 * @code
 * std::vector<Base*> bases = {new Derived(), nullptr, new Base()};
 *
 * // Filter to non-null Derived instances
 * std::vector<Derived*> deriveds;
 * std::copy_if(bases.begin(), bases.end(),
 *              std::back_inserter(deriveds),
 *              IsaAndPresentPred<Derived>);
 * // deriveds = {Derived*} (skips nullptr and Base)
 *
 * // Count non-null Derived1 or Derived2
 * auto count = std::count_if(bases.begin(), bases.end(),
 *                             IsaAndPresentPred<Derived1, Derived2>);
 *
 * // Works with optional
 * std::vector<std::optional<Base>> opts = {...};
 * auto has_derived = std::any_of(opts.begin(), opts.end(),
 *                                 IsaAndPresentPred<Derived>);
 *
 * // C++20 ranges - filter to non-null Derived
 * auto valid_derived = bases
 *     | std::views::filter(IsaAndPresentPred<Derived>)
 *     | std::views::transform([](auto *p) { return cast<Derived>(p); });
 * @endcode
 *
 * @see isa_and_present() for the underlying function
 * @see IsaPred for version that asserts on null
 */
template <typename... Types>
inline constexpr detail::IsaAndPresentCheckPredicate<Types...> IsaAndPresentPred{};

/**
 * @ingroup casting
 * @brief Functor for static_cast operations with algorithms.
 *
 * StaticCastTo is a constexpr variable template that provides a functor
 * wrapping static_cast<To> for use with algorithms like std::transform.
 * Uses perfect forwarding to preserve value categories.
 *
 * This is useful for simple type conversions where you need function object
 * syntax, such as transforming containers or composing with ranges.
 *
 * @tparam To The target type for the cast
 *
 * @note No type checking - performs unchecked static_cast
 * @note Preserves value categories via perfect forwarding
 * @note Use CastTo for checked casting with assertions
 *
 * Example:
 * @code
 * // Numeric conversions
 * std::vector<double> doubles = {1.5, 2.5, 3.5};
 * std::vector<int> ints;
 * std::transform(doubles.begin(), doubles.end(),
 *                std::back_inserter(ints),
 *                StaticCastTo<int>);
 * // ints = {1, 2, 3}
 *
 * // Pointer conversions (use with caution - unchecked!)
 * std::vector<Derived*> deriveds = {...};
 * std::vector<Base*> bases;
 * std::transform(deriveds.begin(), deriveds.end(),
 *                std::back_inserter(bases),
 *                StaticCastTo<Base*>);
 *
 * // C++20 ranges composition
 * auto ints_view = doubles | std::views::transform(StaticCastTo<int>);
 * @endcode
 *
 * @see CastTo for checked casting with assertions
 * @see DynCastTo for dynamic casting with null on failure
 */
template <typename To>
inline constexpr detail::StaticCastFunc<To> StaticCastTo{};

/**
 * @ingroup casting
 * @brief Functor for checked cast operations with algorithms.
 *
 * CastTo is a constexpr variable template that provides a functor wrapping
 * cast<To> for use with algorithms. Performs type-safe downcast with runtime
 * type checking and assertion on mismatch.
 *
 * Unlike StaticCastTo, this functor performs runtime type checking using the
 * classof() pattern and asserts if the cast is invalid. Use with algorithms
 * when you know the types are correct and want to catch errors in debug builds.
 *
 * @tparam To The target type for the cast
 *
 * @note Asserts on type mismatch (debug::Assert)
 * @note Requires To to implement classof() for type checking
 * @note Preserves value categories via perfect forwarding
 *
 * Example:
 * @code
 * std::vector<Base*> bases = {new Derived(), new Derived()};
 *
 * // Transform to derived (asserts if any Base is not Derived)
 * std::vector<Derived*> deriveds;
 * std::transform(bases.begin(), bases.end(),
 *                std::back_inserter(deriveds),
 *                CastTo<Derived*>);
 *
 * // Use after filtering to ensure all are correct type
 * auto derived_ptrs = bases
 *     | std::views::filter(IsaPred<Derived>)
 *     | std::views::transform(CastTo<Derived*>);
 *
 * // Works with references
 * std::vector<std::reference_wrapper<Base>> refs = {...};
 * std::vector<Derived*> ptrs;
 * std::transform(refs.begin(), refs.end(),
 *                std::back_inserter(ptrs),
 *                [](Base &b) { return &CastTo<Derived&>(b); });
 * @endcode
 *
 * @see cast() for the underlying function
 * @see DynCastTo for version that returns nullptr on failure
 * @see CastIfPresentTo for null-safe version
 */
template <typename To>
inline constexpr detail::CastFunc<To> CastTo{};

/**
 * @ingroup casting
 * @brief Functor for null-safe checked cast operations with algorithms.
 *
 * CastIfPresentTo is a constexpr variable template that provides a functor
 * wrapping cast_if_present<To> for use with algorithms. Combines presence
 * checking with type-safe casting - returns null if value is not present,
 * asserts if present but wrong type.
 *
 * This is useful when transforming containers with nullable types (pointers,
 * optionals) where you want null-safety but still want assertions on type
 * mismatches for non-null values.
 *
 * @tparam To The target type for the cast
 *
 * @note Returns null/empty if value is not present
 * @note Asserts if value is present but not of type To
 * @note Preserves value categories via perfect forwarding
 *
 * Example:
 * @code
 * std::vector<Base*> bases = {new Derived(), nullptr, new Derived()};
 *
 * // Transform with null-safety (nullptr stays nullptr)
 * std::vector<Derived*> deriveds;
 * std::transform(bases.begin(), bases.end(),
 *                std::back_inserter(deriveds),
 *                CastIfPresentTo<Derived*>);
 * // deriveds = {Derived*, nullptr, Derived*}
 *
 * // Works with optional
 * std::vector<std::optional<Base>> opts = {Derived(), {}, Derived()};
 * std::vector<std::optional<Derived>> derived_opts;
 * std::transform(opts.begin(), opts.end(),
 *                std::back_inserter(derived_opts),
 *                CastIfPresentTo<Derived>);
 *
 * // Compose with filtering
 * auto valid_deriveds = bases
 *     | std::views::transform(CastIfPresentTo<Derived*>)
 *     | std::views::filter([](auto *p) { return p != nullptr; });
 * @endcode
 *
 * @see cast_if_present() for the underlying function
 * @see CastTo for version that asserts on null
 * @see DynCastIfPresentTo for version that returns null on type mismatch
 */
template <typename To>
inline constexpr detail::CastIfPresentFunc<To> CastIfPresentTo{};

/**
 * @ingroup casting
 * @brief Functor for null-safe dynamic cast operations with algorithms.
 *
 * DynCastIfPresentTo is a constexpr variable template that provides a functor
 * wrapping dyn_cast_if_present<To> for use with algorithms. This is the safest
 * casting functor - it returns null if the value is not present OR if the type
 * is wrong, with no assertions.
 *
 * Use this when transforming containers where both null values and type
 * mismatches should be handled gracefully by returning null rather than
 * asserting or throwing.
 *
 * @tparam To The target type for the cast
 *
 * @note No assertions - completely safe
 * @note Returns null if value is not present
 * @note Returns null if value is present but wrong type
 * @note Ideal for filtering pipelines that discard failures
 *
 * Example:
 * @code
 * std::vector<Base*> bases = {new Derived(), nullptr, new OtherType()};
 *
 * // Transform with full safety (nullptr and wrong types become nullptr)
 * std::vector<Derived*> deriveds;
 * std::transform(bases.begin(), bases.end(),
 *                std::back_inserter(deriveds),
 *                DynCastIfPresentTo<Derived*>);
 * // deriveds = {Derived*, nullptr, nullptr}
 *
 * // Filter out nulls to get only successful casts
 * std::vector<Derived*> valid_deriveds;
 * for (auto *d : bases | std::views::transform(DynCastIfPresentTo<Derived*>)
 *                      | std::views::filter([](auto *p) { return p; })) {
 *     valid_deriveds.push_back(d);
 * }
 *
 * // Count successful casts
 * auto count = std::count_if(bases.begin(), bases.end(),
 *                             [](Base *b) {
 *                                 return DynCastIfPresentTo<Derived*>(b) != nullptr;
 *                             });
 * @endcode
 *
 * @see dyn_cast_if_present() for the underlying function
 * @see DynCastTo for version that asserts on null
 * @see CastIfPresentTo for version that asserts on type mismatch
 */
template <typename To>
inline constexpr detail::DynCastIfPresentFunc<To> DynCastIfPresentTo{};

/**
 * @ingroup casting
 * @brief Functor for dynamic cast operations with algorithms.
 *
 * DynCastTo is a constexpr variable template that provides a functor wrapping
 * dyn_cast<To> for use with algorithms. Performs type-safe downcast with
 * runtime checking - returns nullptr on type mismatch but asserts if the
 * value is null.
 *
 * Use this when transforming containers where type mismatches should be handled
 * gracefully (return nullptr) but null values should be caught (assertion).
 * This is useful when processing containers that should never contain nulls
 * but may contain mixed types.
 *
 * @tparam To The target type for the cast
 *
 * @note Asserts if value is null
 * @note Returns nullptr if value is present but wrong type (no assertion)
 * @note Requires To to implement classof() for type checking
 *
 * Example:
 * @code
 * std::vector<Base*> bases = {new Derived(), new Base(), new Derived()};
 *
 * // Transform with dynamic cast (wrong types become nullptr)
 * std::vector<Derived*> deriveds;
 * std::transform(bases.begin(), bases.end(),
 *                std::back_inserter(deriveds),
 *                DynCastTo<Derived*>);
 * // deriveds = {Derived*, nullptr, Derived*}
 *
 * // Filter to only successful casts
 * auto valid_deriveds = bases
 *     | std::views::transform(DynCastTo<Derived*>)
 *     | std::views::filter([](auto *p) { return p != nullptr; });
 *
 * // Pattern: separate types from mixed container
 * std::vector<Derived1*> d1s, d2s;
 * for (Base *b : bases) {
 *     if (auto *d1 = DynCastTo<Derived1*>(b)) d1s.push_back(d1);
 *     if (auto *d2 = DynCastTo<Derived2*>(b)) d2s.push_back(d2);
 * }
 * @endcode
 *
 * @see dyn_cast() for the underlying function
 * @see DynCastIfPresentTo for null-safe version
 * @see CastTo for version that asserts on type mismatch
 */
template <typename To>
inline constexpr detail::DynCastFunc<To> DynCastTo{};

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_CASTING_PREDICATES_HXX
