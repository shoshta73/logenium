// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGEIUM_CORELIB_EXTENSIBLE_RTTI_EXTENDS_HXX
#define LOGEIUM_CORELIB_EXTENSIBLE_RTTI_EXTENDS_HXX

#include <corelib/internal/tracing.hxx>

namespace corelib::rtti {

/**
 * @brief CRTP base class template for implementing extensible RTTI hierarchies.
 * @ingroup extensible-rtti
 *
 * This template provides automatic implementation of the extensible RTTI interface
 * for derived classes. It uses the Curiously Recurring Template Pattern (CRTP) to
 * inject type-specific implementations of TypeID(), DynamicTypeID(), IsA(), and
 * classof() methods.
 *
 * The template supports both single and multiple inheritance through variadic
 * template parameters. The first parent type is the primary parent, and additional
 * parents can be specified to enable multiple inheritance.
 *
 * @tparam ThisType The derived class type (CRTP parameter)
 * @tparam ParentType The primary parent class type
 * @tparam ParentTypes Additional parent types for multiple inheritance (optional)
 *
 * @note Requirements for ThisType:
 * - Must declare a static member: `static char ID;`
 * - Must define ID in implementation file: `char ThisType::ID = 0;`
 * - Must implement: `void anchor() override {}`
 * - Should declare Extends as friend: `friend class Extends<ThisType, ParentType, ...>;`
 *
 * @note This class automatically provides:
 * - Static and dynamic type identification
 * - Type checking via IsA() methods
 * - Integration with LLVM-style casting (isa, cast, dyn_cast)
 *
 * Single Inheritance Example:
 * @code
 * class MyType : public Extends<MyType, Base> {
 *   public:
 *     MyType() {}
 *   private:
 *     static char ID;
 *     void anchor() override {}
 *     friend class Extends<MyType, Base>;
 * };
 *
 * char MyType::ID = 0;
 * @endcode
 *
 * Multiple Inheritance Example:
 * @code
 * class MyMultiType : public Extends<MyMultiType, ParentA, ParentB> {
 *   public:
 *     MyMultiType() {}
 *   private:
 *     static char ID;
 *     void anchor() override {}
 *     friend class Extends<MyMultiType, ParentA, ParentB>;
 * };
 *
 * char MyMultiType::ID = 0;
 * @endcode
 *
 * @see Base
 */
template <typename ThisT, typename ParentT, typename... ParentTs>
class Extends : public ParentT, public ParentTs... {
  public:
    /**
     * @brief Type alias for the primary parent type.
     *
     * @note This shadows the template parameter name but provides convenient
     * access to the primary parent type within the derived class.
     */
    using ParentType = ParentT;

    /**
     * @brief Returns the static type identifier for ThisType.
     *
     * Returns the address of the static ID member in ThisType. This address
     * serves as a unique compile-time constant identifier for the type.
     *
     * @return Pointer to ThisType::ID
     */
    static const void *TypeID() {
        CRLB_ZONE_SCOPED;
        return &ThisT::ID;
    }

    /**
     * @brief Returns the dynamic type identifier for this object.
     *
     * Overrides the pure virtual DynamicTypeID() from Base to return the
     * type identifier for ThisType. This enables runtime type identification.
     *
     * @return Pointer to ThisType::ID
     */
    [[nodiscard]] const void *DynamicTypeID() const override {
        CRLB_ZONE_SCOPED;
        return &ThisT::ID;
    }

    /**
     * @brief Template-based type checking.
     *
     * Checks if this object is an instance of or derived from QueryType by
     * comparing type identifiers. This is a convenience wrapper around the
     * pointer-based IsA() method.
     *
     * @tparam QueryType The type to check against
     * @return true if this object is of type QueryType or derives from it
     *
     * Example:
     * @code
     * MyType obj;
     * if (obj.IsA<Base>()) {
     *     // obj derives from Base
     * }
     * if (obj.IsA<MyType>()) {
     *     // obj is exactly MyType
     * }
     * @endcode
     */
    template <typename QueryType>
    [[nodiscard]] bool IsA() const {
        CRLB_ZONE_SCOPED;
        return IsA(QueryType::TypeID());
    }

    /**
     * @brief Pointer-based type checking with inheritance chain traversal.
     *
     * Checks if this object is an instance of or derived from the type identified
     * by TID. This method checks the entire inheritance hierarchy including all
     * parent types.
     *
     * For multiple inheritance, this checks:
     * 1. ThisType's TypeID (exact match)
     * 2. Primary parent's inheritance chain
     * 3. All additional parents' inheritance chains (if any)
     *
     * @param TID Type identifier to check against (from TypeID())
     * @return true if this object is of type TID or derives from it
     *
     * @note The implementation uses fold expressions (C++17) to check all parent
     * types in multiple inheritance scenarios.
     */
    bool IsA(const void *const TID) const override {
        CRLB_ZONE_SCOPED;
        return TID == TypeID() || ParentType::IsA(TID) || (ParentTs::IsA(TID) || ...);
    }

    /**
     * @brief LLVM-style classof() method for casting integration.
     *
     * Provides the classof pattern required by corelib's LLVM-style casting
     * infrastructure (isa, cast, dyn_cast). This method enables type checking
     * on pointer types.
     *
     * @tparam Type The pointer type (automatically deduced)
     * @param R Pointer to check
     * @return true if R points to an object of type ThisType or derived from it
     *
     * @note This method is used internally by:
     * - corelib::isa<T>() for type checking
     * - corelib::cast<T>() for checked casting
     * - corelib::dyn_cast<T>() for safe casting
     *
     * Example usage through casting functions:
     * @code
     * Base *ptr = new MyType();
     * if (isa<MyType>(ptr)) {
     *     MyType *typed = cast<MyType>(ptr);  // Uses classof internally
     * }
     * @endcode
     */
    template <typename Type>
    static bool classof(const Type *R) {
        CRLB_ZONE_SCOPED;
        return R->template IsA<ThisT>();
    }
};

}  // namespace corelib::rtti
#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}  // namespace logenium

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGEIUM_CORELIB_EXTENSIBLE_RTTI_EXTENDS_HXX
