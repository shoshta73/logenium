// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGEIUM_CORELIB_EXTENSIBLE_RTTI_BASE_HXX
#define LOGEIUM_CORELIB_EXTENSIBLE_RTTI_BASE_HXX

namespace corelib::rtti {

/**
 * @brief Abstract base class for the extensible RTTI system.
 * @ingroup extensible-rtti
 *
 * This class provides the foundation for a runtime type identification system
 * that works without C++ RTTI. It uses unique type identifiers (addresses of
 * static variables) to enable type checking and dynamic casting.
 *
 * The system is designed to work seamlessly with corelib's LLVM-style casting
 * infrastructure (isa, cast, dyn_cast) through the classof pattern implemented
 * in derived classes using the Extends template.
 *
 * @note Design Patterns Used:
 * - Type ID Pattern: Each type has a unique static ID whose address serves
 *   as a compile-time constant type identifier
 * - Anchor Idiom: Pure virtual anchor() method ensures vtable is emitted in
 *   the implementation file, preventing vtable/RTTI duplication across
 *   translation units
 *
 * @note Usage:
 * Do not inherit from Base directly. Instead, use the Extends template which
 * provides the necessary boilerplate.
 *
 * Example:
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
 * // In .cxx file:
 * char MyType::ID = 0;
 * @endcode
 *
 * @see Extends
 */
class Base {
  public:
    virtual ~Base() = default;

    /**
     * @brief Returns the static type identifier for this class.
     *
     * Each class in the hierarchy has a unique static ID variable. The address
     * of this variable serves as a compile-time constant that uniquely identifies
     * the type. This allows type checking without C++ RTTI.
     *
     * @return Pointer to the static ID variable for Base
     */
    static const void *TypeID() { return &ID; }

    /**
     * @brief Returns the dynamic type identifier for the actual object type.
     *
     * This method must be overridden by all derived classes to return their
     * specific TypeID(). The Extends template automatically provides this
     * implementation.
     *
     * @return Pointer to the static ID variable of the most-derived type
     */
    [[nodiscard]] virtual const void *DynamicTypeID() const = 0;

    /**
     * @brief Checks if this object is an instance of or derived from the given type.
     *
     * This method performs runtime type checking by comparing type IDs. It
     * checks if the object's dynamic type matches the queried type or any of
     * its base types.
     *
     * The default implementation only checks against Base's TypeID. Derived
     * classes using Extends will override this to check the entire inheritance
     * chain.
     *
     * @param TID Type identifier to check against (obtained from TypeID())
     * @return true if this object is of type TID or derives from it, false otherwise
     *
     * Example:
     * @code
     * Base *obj = new MyDerivedType();
     * if (obj->IsA(MyDerivedType::TypeID())) {
     *     // obj is a MyDerivedType
     * }
     * @endcode
     */
    virtual bool IsA(const void *const TID) const { return TID == TypeID(); };

  private:
    /**
     * @brief Anchor method to ensure vtable is emitted in implementation file.
     *
     * This pure virtual method forces derived classes to provide an implementation,
     * which ensures the vtable is emitted in a single translation unit rather than
     * being duplicated across multiple object files.
     *
     * @note Derived classes using Extends should provide an empty implementation:
     * @code
     * void anchor() override {}
     * @endcode
     */
    virtual void anchor() = 0;

    /**
     * @brief Static type identifier for Base.
     *
     * The address of this variable serves as the unique identifier for the Base
     * type. The actual value is irrelevant; only the address matters.
     */
    static char ID;
};

}  // namespace corelib::rtti

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}  // namespace logenium

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGEIUM_CORELIB_EXTENSIBLE_RTTI_BASE_HXX
