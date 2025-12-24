// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

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
