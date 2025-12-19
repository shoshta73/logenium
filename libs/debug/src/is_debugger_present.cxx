#include "debug/is_debugger_present.hxx"

#include <xheader/windows.h>

namespace debug {

#ifndef NDEBUG

bool IsDebuggerPresent() {
#ifdef _WIN32

    return ::IsDebuggerPresent() != 0;

#else

    return false;

#endif
}

#endif  // NDEBUG

}  // namespace debug
