#ifndef LOGENIUM_DEBUG_IS_DEBUGGER_PRESENT_HXX
#define LOGENIUM_DEBUG_IS_DEBUGGER_PRESENT_HXX

namespace debug {

#ifndef NDEBUG

bool IsDebuggerPresent();

#else

consteval bool IsDebuggerPresent() { return false; }

#endif

}  // namespace debug

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::debug;

}

#endif

#endif  // LOGENIUM_DEBUG_IS_DEBUGGER_PRESENT_HXX
