// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_UTILITY_TYPE_NAME_HXX
#define LOGENIUM_CORELIB_UTILITY_TYPE_NAME_HXX

#include <string_view>
#include <type_traits>

namespace corelib {

/**
 * @ingroup utility
 * @brief Extract the type name of a template parameter at compile time.
 *
 * This function uses compiler intrinsics (__PRETTY_FUNCTION__ for GCC/Clang,
 * __FUNCSIG__ for MSVC) to extract the type name as a string view at compile time.
 *
 * @tparam T The type to extract the name from
 * @return constexpr std::string_view The type name as a string view
 *
 * @note This function is constexpr and can be evaluated at compile time
 * @note Requires Clang, GCC, or MSVC compiler
 *
 * Example:
 * @code
 * constexpr auto name = type_name<int>();  // Returns "int"
 * constexpr auto name2 = type_name<const std::string>();  // Returns "const std::basic_string<char>"
 * @endcode
 */
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

/**
 * @ingroup utility
 * @brief Extract the type name from a value at compile time.
 *
 * This overload takes a forwarding reference to preserve cv-qualifiers and
 * extracts the type name by removing the reference and calling the template
 * version.
 *
 * @tparam T The deduced type from the value (automatically deduced)
 * @param value The value to extract the type name from (not evaluated)
 * @return constexpr std::string_view The type name as a string view
 *
 * @note This function is constexpr and can be evaluated at compile time
 * @note The value parameter is not evaluated, only used for type deduction
 * @note Reference qualifiers are stripped, but cv-qualifiers are preserved
 *
 * Example:
 * @code
 * int x = 42;
 * const int y = 10;
 * auto name1 = type_name(x);  // Returns "int"
 * auto name2 = type_name(y);  // Returns "int" (const is part of type, not reference)
 * auto name3 = type_name(std::move(x));  // Returns "int"
 * @endcode
 *
 * @todo Implement not stripping reference qualifiers
 */
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
