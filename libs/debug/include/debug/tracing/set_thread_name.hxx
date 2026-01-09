// SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
// SPDX-License-Identifier: BSD-3-Clause

/**
 * @file
 * @brief Thread naming utilities for profiling and debugging
 *
 * This header provides functions to set thread names that are visible in profiling
 * tools. Thread names improve the developer experience when analyzing performance
 * or debugging multi-threaded applications by providing meaningful identification
 * of threads in profiler output.
 */

#ifndef LOGENIUM_DEBUG_TRACING_SET_THREAD_NAME_HXX
#define LOGENIUM_DEBUG_TRACING_SET_THREAD_NAME_HXX

#include <string_view>

namespace debug::tracing {

/**
 * @brief Set the name of the current thread
 * @ingroup tracing
 *
 * Sets a human-readable name for the current thread that will be visible in
 * profiling tools. This helps identify threads during performance analysis and
 * debugging sessions. The thread name is passed directly to Tracy profiler.
 *
 * @param name The name to assign to the current thread (null-terminated expected)
 *
 * @note The name string should remain valid for the lifetime of the thread or
 *       until another name is set, as Tracy may retain a pointer to it.
 * @note This function is always available regardless of build configuration,
 *       as Tracy is always linked via TracyClient.
 *
 * @code
 * #include <debug/tracing/set_thread_name.hxx>
 *
 * void WorkerThread() {
 *     debug::tracing::SetThreadName("Worker-1");
 *     // Perform work...
 * }
 * @endcode
 */
void SetThreadName(std::string_view name);

/**
 * @brief Set the name of the current thread with a group hint
 * @ingroup tracing
 *
 * Sets a human-readable name for the current thread along with a group hint
 * for organizational purposes in profiling tools. The group hint allows related
 * threads to be visually grouped together in Tracy's thread view, improving
 * readability when dealing with many threads.
 *
 * @param name The name to assign to the current thread (null-terminated expected)
 * @param group_hint An integer hint used by the profiler to group related threads
 *
 * @note The name string should remain valid for the lifetime of the thread or
 *       until another name is set, as Tracy may retain a pointer to it.
 * @note The group hint is used by Tracy to organize threads visually. Threads
 *       with the same group hint will be displayed together.
 * @note This function is always available regardless of build configuration,
 *       as Tracy is always linked via TracyClient.
 *
 * @code
 * #include <debug/tracing/set_thread_name.hxx>
 *
 * // Group rendering threads together
 * void RenderThread(int id) {
 *     debug::tracing::SetThreadName("Renderer-" + std::to_string(id), 1);
 *     // Perform rendering...
 * }
 *
 * // Group worker threads together
 * void WorkerThread(int id) {
 *     debug::tracing::SetThreadName("Worker-" + std::to_string(id), 2);
 *     // Perform work...
 * }
 * @endcode
 */
void SetThreadName(std::string_view name, int group_hint);

}  // namespace debug::tracing

#ifdef __LOGENIUM_SOURCE__

namespace logenium {

using namespace ::debug;

}  // namespace logenium

#endif

#endif  // LOGENIUM_DEBUG_TRACING_SET_THREAD_NAME_HXX
