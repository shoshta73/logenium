// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_UTILITY_DEFER_HXX
#define LOGENIUM_CORELIB_UTILITY_DEFER_HXX

#include <utility>

namespace corelib {

template <typename Functor>
struct Defer {
    constexpr Defer(Functor &&functor) : functor(std::move(functor)) {}

    ~Defer() { functor(); }

  private:
    Functor functor;
};

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_UTILITY_DEFER_HXX
