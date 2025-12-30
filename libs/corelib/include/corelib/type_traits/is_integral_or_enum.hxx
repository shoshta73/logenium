// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_TYPE_TRAITS_IS_INTEGRAL_OR_ENUM_HXX
#define LOGENIUM_CORELIB_TYPE_TRAITS_IS_INTEGRAL_OR_ENUM_HXX

#include <type_traits>

namespace corelib {

template <typename T>
class is_integral_or_enum {
    using UnderlyingT = std::remove_reference_t<T>;

  public:
    static const bool value = !std::is_class_v<UnderlyingT> &&  // Filter conversion operators.
                              !std::is_pointer_v<UnderlyingT> && !std::is_floating_point_v<UnderlyingT> &&
                              (std::is_enum_v<UnderlyingT> || std::is_convertible_v<UnderlyingT, unsigned long long>);
};

template <typename T>
inline constexpr bool is_integral_or_enum_v = is_integral_or_enum<T>::value;

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_TYPE_TRAITS_IS_INTEGRAL_OR_ENUM_HXX
