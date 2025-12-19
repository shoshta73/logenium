#ifndef LOGENIUM_DEBUG_BREAKPOINT_HXX
#define LOGENIUM_DEBUG_BREAKPOINT_HXX

namespace debug {

#ifndef NDEBUG

void Breakpoint();

#else

consteval void Breakpoint() {}

#endif

}  // namespace debug

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::debug;

}

#endif

#endif  // LOGENIUM_DEBUG_BREAKPOINT_HXX
