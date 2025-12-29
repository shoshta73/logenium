// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_UTILITY_TYPE_NAME_HXX
#define LOGENIUM_CORELIB_UTILITY_TYPE_NAME_HXX

#include <string_view>
#include <type_traits>

namespace corelib {

template <typename T>
constexpr std::string_view type_name() {
#if defined(__clang__) || defined(__GNUC__)

    constexpr auto prefix = std::string_view{"="};
    constexpr auto suffix = std::string_view{"]"};
    constexpr auto function = std::string_view{__PRETTY_FUNCTION__};

    constexpr auto first = function.find_first_not_of(' ', function.find_first_of(prefix) + 1);
    constexpr auto value = function.substr(first, function.find_last_of(suffix) - first);
    return value;

#elif defined(_MSC_VER)

    constexpr auto function = std::string_view{__FUNCSIG__};
    constexpr auto prefix = std::string_view{"type_name<"};
    constexpr auto suffix = std::string_view{">(void)"};

    constexpr auto start = function.find(prefix) + prefix.size();
    constexpr auto end = function.rfind(suffix);

    constexpr auto value = function.substr(start, end - start);
    return value;

#else
#error "corelib/type_name.hxx: Unsupported compiler"
#endif
}

template <typename T>
inline constexpr std::string_view type_name(T &&value) {
    using Type = std::remove_reference_t<T>;
    return type_name<Type>();
}

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_UTILITY_TYPE_NAME_HXX
