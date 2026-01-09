#include <string_view>

#include <common/TracySystem.hpp>
#include <tracy/Tracy.hpp>

#include <debug/tracing/set_thread_name.hxx>

namespace debug::tracing {

void SetThreadName(std::string_view name) { tracy::SetThreadName(name.data()); }

void SetThreadName(std::string_view name, int group_hint) { tracy::SetThreadNameWithHint(name.data(), group_hint); }

}  // namespace debug::tracing
