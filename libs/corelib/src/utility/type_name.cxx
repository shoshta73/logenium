// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#include <string_view>

#include <corelib/utility/type_name.hxx>

namespace corelib {

#if defined(_MSC_VER)

/**
 * @ingroup utility
 * @brief MSVC-specific specialization for __int64 to normalize to "long long".
 *
 * MSVC uses __int64 as the underlying type, but this specialization normalizes
 * it to "long long" for consistency across compilers.
 *
 * @return constexpr std::string_view "long long"
 */
template <>
inline constexpr std::string_view type_name<__int64>() {
    return {"long long"};
}

/**
 * @ingroup utility
 * @brief MSVC-specific specialization for unsigned __int64 to normalize to "unsigned long long".
 *
 * MSVC uses unsigned __int64 as the underlying type, but this specialization
 * normalizes it to "unsigned long long" for consistency across compilers.
 *
 * @return constexpr std::string_view "unsigned long long"
 */
template <>
inline constexpr std::string_view type_name<unsigned __int64>() {
    return {"unsigned long long"};
}

#endif

}  // namespace corelib
