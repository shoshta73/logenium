#include "debug/breakpoint.hxx"

#include <csignal>  // IWYU pragma: keep // for std::raise

#include <xheader/windows.h>

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

namespace debug {

#ifndef NDEBUG

void Breakpoint() {
#if __has_builtin(__builtin_debugtrap)

    __builtin_debugtrap();

#elif defined(_MSC_VER)

    __debugbreak();

#else

#ifdef _WIN32

    DebugBreak();

#else

    std::raise(SIGTRAP);

#endif

#endif  // __has_builtin(__builtin_debugtrap)
}

#endif  // NDEBUG

}  // namespace debug
