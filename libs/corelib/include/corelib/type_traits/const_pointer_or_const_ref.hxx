// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_TYPE_TRAITS_CONST_POINTER_OR_CONST_REF_HXX
#define LOGENIUM_CORELIB_TYPE_TRAITS_CONST_POINTER_OR_CONST_REF_HXX

#include <type_traits>

namespace corelib {

template <typename T>
struct const_pointer_or_const_ref {
    using type = std::conditional_t<std::is_pointer_v<T>, const T, const T &>;
};

template <typename T>
using const_pointer_or_const_ref_t = typename const_pointer_or_const_ref<T>::type;

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_TYPE_TRAITS_CONST_POINTER_OR_CONST_REF_HXX
