// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_CASTING_HXX
#define LOGENIUM_CORELIB_CASTING_HXX

/**
 * @file casting.hxx
 * @brief Convenience header for all casting module components
 *
 * This header includes all RTTI-free casting utilities: type checking (isa),
 * checked casts (cast), dynamic casts (dyn_cast), and predicate functors.
 * Include this file to get access to the complete casting infrastructure.
 */

/**
 * @defgroup casting Casting Module
 * @brief RTTI-free casting utilities inspired by LLVM
 *
 * The casting module provides a complete infrastructure for runtime type queries
 * and type conversions without using C++ RTTI. This is achieved through the
 * `classof()` pattern where types implement static type-checking methods.
 *
 * ## Features
 *
 * - **Zero overhead**: No vtables or RTTI required
 * - **Type safe**: Compile-time type checking with runtime validation
 * - **Flexible**: Supports pointers, references, std::unique_ptr, std::optional
 * - **Const correct**: Preserves const-qualification throughout casts
 * - **SFINAE friendly**: Works with template metaprogramming
 *
 * ## Core Operations
 *
 * ### Type Checking
 * - `isa<T>(val)`: Check if value is of type T (supports variadic)
 * - `isa_and_present<T>(val)`: Null-safe type checking
 *
 * ### Checked Casts
 * - `cast<T>(val)`: Cast with assertion on failure (debug builds)
 * - `cast_if_present<T>(val)`: Null-safe checked cast
 * - `cast_or_null<T>(val)`: Returns nullptr on failure
 *
 * ### Dynamic Casts
 * - `dyn_cast<T>(val)`: Returns nullptr on failure
 * - `dyn_cast_if_present<T>(val)`: Null-safe dynamic cast
 * - `dyn_cast_or_null<T>(val)`: Explicit nullable variant
 * - `unique_dyn_cast<T>(ptr)`: For std::unique_ptr ownership transfer
 *
 * ### Predicate Functors
 * - `IsaPred<T>`: Type checking predicate for algorithms
 * - `CastTo<T>`, `DynCastTo<T>`: Casting functors for transformations
 *
 * ## Requirements
 *
 * Types must implement `static bool classof(const Base *)` for RTTI-free
 * type checking:
 *
 * @code
 * class Base {
 * public:
 *     enum Kind { K_Base, K_Derived };
 *     Kind getKind() const { return kind; }
 * protected:
 *     Base(Kind k) : kind(k) {}
 * private:
 *     Kind kind;
 * };
 *
 * class Derived : public Base {
 * public:
 *     Derived() : Base(K_Derived) {}
 *     static bool classof(const Base *b) {
 *         return b->getKind() == K_Derived;
 *     }
 * };
 * @endcode
 *
 * ## Usage
 *
 * @code
 * Base *b = new Derived();
 *
 * // Type checking
 * if (isa<Derived>(b)) {
 *     // b is a Derived
 * }
 *
 * // Checked cast (asserts in debug)
 * auto &d = cast<Derived>(b);
 *
 * // Dynamic cast (returns nullptr on failure)
 * if (auto *d = dyn_cast<Derived>(b)) {
 *     // Use d
 * }
 *
 * // Variadic type checking
 * if (isa<Derived1, Derived2>(b)) {
 *     // b is either Derived1 or Derived2
 * }
 * @endcode
 *
 * Include the convenience header:
 * @code
 * #include <corelib/casting.hxx>
 * @endcode
 *
 * Or include specific headers:
 * @code
 * #include <corelib/casting/isa.hxx>
 * #include <corelib/casting/cast.hxx>
 * #include <corelib/casting/dyn_cast.hxx>
 * #include <corelib/casting/predicates.hxx>
 * @endcode
 */

#include <corelib/casting/cast.hxx>        // IWYU pragma: export
#include <corelib/casting/dyn_cast.hxx>    // IWYU pragma: export
#include <corelib/casting/isa.hxx>         // IWYU pragma: export
#include <corelib/casting/predicates.hxx>  // IWYU pragma: export

#endif  // LOGENIUM_CORELIB_CASTING_HXX
