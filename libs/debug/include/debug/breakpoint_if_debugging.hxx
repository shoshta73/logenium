#ifndef LOGENIUM_DEBUG_BREAKPOINT_IF_DEBUGGING_HXX
#define LOGENIUM_DEBUG_BREAKPOINT_IF_DEBUGGING_HXX

namespace debug {

#ifndef NDEBUG

void BreakpointIfDebugging();

#else

consteval void BreakpointIfDebugging() {}

#endif

}  // namespace debug

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::debug;

}

#endif

#endif  // LOGENIUM_DEBUG_BREAKPOINT_IF_DEBUGGING_HXX
