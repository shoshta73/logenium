// SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LOGENIUM_CORELIB_UTILITY_DEFER_HXX
#define LOGENIUM_CORELIB_UTILITY_DEFER_HXX

#include <utility>

#include <corelib/internal/tracing.hxx>

namespace corelib {

/**
 * @ingroup utility
 * @brief RAII wrapper for executing a functor at scope exit.
 *
 * Defer is a simple RAII utility that executes a given functor when the Defer
 * object is destroyed (at scope exit). This is useful for cleanup operations,
 * ensuring resources are released even in the presence of early returns or
 * exceptions.
 *
 * @tparam Functor The type of the callable object (lambda, function object, etc.)
 *
 * @note Multiple Defer objects in the same scope execute in LIFO order (reverse
 *       construction order)
 * @note This class is move-only (not copyable)
 *
 * Example:
 * @code
 * void process_file(const char *filename) {
 *     FILE *file = fopen(filename, "r");
 *     if (!file) return;
 *
 *     // Automatically close file at scope exit
 *     Defer cleanup([file]() { fclose(file); });
 *
 *     // ... use file ...
 *     // file is automatically closed here, even if exceptions are thrown
 * }
 *
 * // Multiple defers execute in reverse order:
 * {
 *     Defer d1([]() { std::cout << "3\n"; });
 *     Defer d2([]() { std::cout << "2\n"; });
 *     Defer d3([]() { std::cout << "1\n"; });
 *     // Prints: 1, 2, 3
 * }
 * @endcode
 */
template <typename Functor>
struct Defer {
    /**
     * @brief Construct a Defer object with a functor to execute at scope exit.
     *
     * @param functor The callable object to execute in the destructor (moved).
     *                Should be noexcept or handle exceptions internally to avoid
     *                std::terminate when called from the destructor.
     */
    constexpr Defer(Functor &&functor) : functor(std::move(functor)) {}

    /**
     * @brief Destructor that executes the stored functor.
     */
    ~Defer() {
        CRLB_ZONE_SCOPED;
        functor();
    }

  private:
    /**
     * @brief The callable object to execute at scope exit.
     *
     * @note Should be noexcept or handle exceptions internally to avoid
     *       std::terminate when called from the destructor.
     */
    Functor functor;
};

}  // namespace corelib

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::corelib;

}

#endif  // __LOGENIUM_SOURCE__

#endif  // LOGENIUM_CORELIB_UTILITY_DEFER_HXX
