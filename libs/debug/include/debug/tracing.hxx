// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * @brief Umbrella header for tracing utilities
 *
 * This header provides access to all tracing functionality in the debug library.
 * The tracing module offers thread naming utilities via Tracy profiler integration,
 * enabling better visibility in profiling tools.
 *
 * @defgroup tracing Tracing Utilities
 * @brief Thread naming and profiling utilities via Tracy integration
 *
 * The tracing module provides utilities for profiling and performance analysis using
 * Tracy profiler (v0.13.0). It includes thread naming functions and scope-based
 * profiling zone macros that help identify performance bottlenecks and thread behavior
 * during development.
 *
 * Key features:
 * - Thread naming with optional group hints for organization
 * - Scope-based profiling zones (debug builds only, zero overhead in release)
 * - Integration with Tracy profiler for visualization
 * - Thread naming always available, zone macros conditional on NDEBUG
 *
 * @code
 * #include <debug/tracing.hxx>
 *
 * void WorkerThread() {
 *     debug::tracing::SetThreadName("Worker-1");
 *     ZoneScoped;  // Profile entire function (debug builds only)
 *     // Thread operations...
 * }
 *
 * void ProcessData(const Data& data) {
 *     ZoneScopedN("Data Processing");  // Custom zone name
 *
 *     {
 *         ZoneScopedN("Validation");
 *         // Validation logic...
 *     }
 *
 *     {
 *         ZoneScopedN("Transformation");
 *         // Transformation logic...
 *     }
 * }
 * @endcode
 */

#ifndef LOGENIUM_DEBUG_TRACING_HXX
#define LOGENIUM_DEBUG_TRACING_HXX

#include <debug/tracing/macros.hxx>
#include <debug/tracing/set_thread_name.hxx>

#endif  // LOGENIUM_DEBUG_TRACING_HXX
