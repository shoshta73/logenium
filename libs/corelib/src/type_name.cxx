// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <string_view>

#include <corelib/type_name.hxx>

namespace corelib {

#if defined(_MSC_VER)

template <>
inline constexpr std::string_view type_name<__int64>() {
    return {"long long"};
}

template <>
inline constexpr std::string_view type_name<unsigned __int64>() {
    return {"unsigned long long"};
}

#endif

}  // namespace corelib
