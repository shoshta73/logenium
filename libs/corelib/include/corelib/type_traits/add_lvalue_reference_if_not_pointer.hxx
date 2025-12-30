// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_TYPE_TRAITS_ADD_LVALUE_REFERENCE_IF_NOT_POINTER_HXX
#define LOGENIUM_CORELIB_TYPE_TRAITS_ADD_LVALUE_REFERENCE_IF_NOT_POINTER_HXX

#include <type_traits>

namespace corelib {

template <typename T>
struct add_lvalue_reference_if_not_pointer {
    using type = std::conditional_t<std::is_pointer_v<T>, T, T &>;
};

template <typename T>
using add_lvalue_reference_if_not_pointer_t = typename add_lvalue_reference_if_not_pointer<T>::type;

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_TYPE_TRAITS_ADD_LVALUE_REFERENCE_IF_NOT_POINTER_HXX
