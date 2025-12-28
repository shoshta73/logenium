#include <corelib/type_name.hxx>

#include <string_view>

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
