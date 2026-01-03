// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGEIUM_CORELIB_EXTENSIBLE_RTTI_HXX
#define LOGEIUM_CORELIB_EXTENSIBLE_RTTI_HXX

/**
 * @file extensible-rtti.hxx
 * @brief Convenience header for all extensible RTTI module components
 *
 * This header includes all extensible RTTI components: the Base class for
 * RTTI hierarchies and the Extends CRTP template for implementing derived
 * classes. Include this file to get access to the complete extensible RTTI
 * system.
 */

/**
 * @defgroup extensible-rtti Extensible RTTI Module
 * @brief RTTI-free runtime type identification and casting system
 *
 * The extensible RTTI module provides a lightweight alternative to C++ RTTI
 * that integrates seamlessly with corelib's LLVM-style casting infrastructure.
 * It uses compile-time type identifiers (addresses of static variables) to
 * enable runtime type checking and safe dynamic casting without vtable overhead.
 *
 * Key features:
 * - **No C++ RTTI required**: Works with `-fno-rtti` compiler flag
 * - **Zero runtime overhead**: Type IDs are compile-time constants
 * - **LLVM-style casting**: Full integration with isa<>, cast<>, dyn_cast<>
 * - **Multiple inheritance**: Support for complex type hierarchies
 * - **CRTP-based**: Automatic method generation via Extends template
 *
 * ## Architecture
 *
 * The system consists of two main components:
 *
 * ### Base Class
 * Abstract base class providing the RTTI interface:
 * - `TypeID()`: Static type identifier
 * - `DynamicTypeID()`: Runtime type identifier
 * - `IsA()`: Type checking method
 *
 * ### Extends Template
 * CRTP template that automatically implements the RTTI interface:
 * - Injects type-specific TypeID implementation
 * - Overrides DynamicTypeID() with derived type
 * - Provides IsA<T>() template for convenient type checking
 * - Implements classof() for LLVM-style casting integration
 *
 * ## Usage
 *
 * ### Basic Hierarchy
 *
 * @code
 * #include <corelib/extensible-rtti.hxx>
 *
 * // Base of your hierarchy
 * class MyBase : public corelib::rtti::Extends<MyBase, corelib::rtti::Base> {
 *   public:
 *     MyBase() {}
 *   private:
 *     static char ID;
 *     void anchor() override {}
 *     friend class corelib::rtti::Extends<MyBase, corelib::rtti::Base>;
 * };
 *
 * // Derived class
 * class MyDerived : public corelib::rtti::Extends<MyDerived, MyBase> {
 *   public:
 *     MyDerived() {}
 *   private:
 *     static char ID;
 *     void anchor() override {}
 *     friend class corelib::rtti::Extends<MyDerived, MyBase>;
 * };
 *
 * // Implementation (.cxx file)
 * char MyBase::ID = 0;
 * char MyDerived::ID = 0;
 * @endcode
 *
 * ### Type Checking
 *
 * @code
 * MyDerived obj;
 * corelib::rtti::Base *base = &obj;
 *
 * // Using IsA() method
 * if (base->IsA(MyDerived::TypeID())) {
 *     // base points to MyDerived
 * }
 *
 * // Using template IsA<>()
 * if (obj.IsA<MyBase>()) {
 *     // obj is derived from MyBase
 * }
 * @endcode
 *
 * ### LLVM-Style Casting
 *
 * @code
 * using namespace corelib;
 *
 * MyBase *base = new MyDerived();
 *
 * // Type checking
 * if (isa<MyDerived>(base)) {
 *     // base points to MyDerived
 * }
 *
 * // Checked cast (asserts on failure in debug builds)
 * MyDerived *derived = cast<MyDerived>(base);
 *
 * // Safe cast (returns nullptr on failure)
 * if (MyDerived *derived = dyn_cast<MyDerived>(base)) {
 *     // Successfully cast to MyDerived
 * }
 * @endcode
 *
 * ### Multiple Inheritance
 *
 * @code
 * class InterfaceA : public corelib::rtti::Extends<InterfaceA, corelib::rtti::Base> {
 *     // ... interface methods ...
 *   private:
 *     static char ID;
 *     void anchor() override {}
 *     friend class corelib::rtti::Extends<InterfaceA, corelib::rtti::Base>;
 * };
 *
 * class InterfaceB : public corelib::rtti::Extends<InterfaceB, corelib::rtti::Base> {
 *     // ... interface methods ...
 *   private:
 *     static char ID;
 *     void anchor() override {}
 *     friend class corelib::rtti::Extends<InterfaceB, corelib::rtti::Base>;
 * };
 *
 * // Multiple inheritance: inherits from both InterfaceA and InterfaceB
 * class Implementation : public corelib::rtti::Extends<Implementation, InterfaceA, InterfaceB> {
 *   public:
 *     Implementation() {}
 *   private:
 *     static char ID;
 *     void anchor() override {}
 *     friend class corelib::rtti::Extends<Implementation, InterfaceA, InterfaceB>;
 * };
 *
 * char InterfaceA::ID = 0;
 * char InterfaceB::ID = 0;
 * char Implementation::ID = 0;
 * @endcode
 *
 * ## Design Patterns
 *
 * ### Type ID Pattern
 * Each type has a unique static `char ID` member. The address of this variable
 * serves as a compile-time constant identifier. Since each type has its own
 * ID variable, the addresses are guaranteed to be unique.
 *
 * ### Anchor Idiom
 * The pure virtual `anchor()` method ensures the vtable is emitted in a single
 * translation unit, preventing vtable duplication and reducing binary size.
 *
 * ### CRTP (Curiously Recurring Template Pattern)
 * The Extends template uses CRTP to inject type-specific implementations at
 * compile time, eliminating the need for manual boilerplate in each derived class.
 *
 * ## Integration with Casting
 *
 * The extensible RTTI system provides the `classof()` method required by
 * corelib's LLVM-style casting infrastructure. This enables seamless use
 * of `isa<>`, `cast<>`, `dyn_cast<>`, and casting predicates with RTTI
 * hierarchies.
 *
 * @see corelib::rtti::Base
 * @see corelib::rtti::Extends
 * @see @ref casting
 */

#include <corelib/extensible-rtti/base.hxx>     // IWYU pragma: export
#include <corelib/extensible-rtti/extends.hxx>  // IWYU pragma: export

#endif  // LOGEIUM_CORELIB_EXTENSIBLE_RTTI_HXX
