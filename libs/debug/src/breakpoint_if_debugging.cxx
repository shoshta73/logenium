#include "debug/breakpoint_if_debugging.hxx"

#include "debug/breakpoint.hxx"           // IWYU pragma: keep // for Breakpoint
#include "debug/is_debugger_present.hxx"  // IWYU pragma: keep // for IsDebuggerPresent

namespace debug {

#ifndef NDEBUG

void BreakpointIfDebugging() {
    if (IsDebuggerPresent()) {
        Breakpoint();
    }
}

#endif  // NDEBUG

}  // namespace debug
