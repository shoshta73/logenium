/**
 * @file
 * @brief Profiling zone macros for scope-based performance measurement
 *
 * This header provides convenience macros for marking profiling zones using Tracy.
 * Zones are automatically tracked in debug builds and become no-ops in release builds,
 * allowing zero-overhead profiling instrumentation in production code.
 */

#ifndef LOGENIUM_DEBUG_TRACING_MACROS_HXX
#define LOGENIUM_DEBUG_TRACING_MACROS_HXX

#include <tracy/Tracy.hpp>

// Save Tracy's original macros before redefining them
#ifdef ZoneScoped
#define __tray_zone_scoped ZoneScoped
#undef ZoneScoped
#endif

#ifdef ZoneScopedN
#define __tray_zone_scoped_named ZoneScopedN
#undef ZoneScopedN
#endif

#ifndef NDEBUG

/**
 * @brief Mark the current scope as a profiling zone
 * @ingroup tracing
 *
 * Creates a profiling zone that spans the current scope. The zone is automatically
 * named based on the function name and is visible in Tracy profiler. When the scope
 * exits, the zone measurement completes. This macro is only active in debug builds
 * (when NDEBUG is not defined) and becomes a no-op in release builds.
 *
 * @note This macro must be placed at the beginning of a scope (function or block).
 * @note Multiple ZoneScoped macros in nested scopes create nested zones in Tracy.
 * @note In release builds (NDEBUG defined), this macro expands to nothing, ensuring
 *       zero runtime overhead in production code.
 *
 * @code
 * #include <debug/tracing/macros.hxx>
 *
 * void ProcessData() {
 *     ZoneScoped;  // Marks entire function as a zone
 *     // Function implementation...
 * }
 *
 * void ComplexOperation() {
 *     ZoneScoped;
 *     {
 *         ZoneScoped;  // Nested zone for a specific block
 *         // Complex operation...
 *     }
 * }
 * @endcode
 */
#define ZoneScoped __tray_zone_scoped

/**
 * @brief Mark the current scope as a profiling zone with a custom name
 * @ingroup tracing
 *
 * Creates a profiling zone with a custom name that spans the current scope. This
 * allows you to give meaningful names to profiling zones instead of relying on
 * automatic function name detection. When the scope exits, the zone measurement
 * completes. This macro is only active in debug builds (when NDEBUG is not defined)
 * and becomes a no-op in release builds.
 *
 * @param name A string literal to use as the zone name
 *
 * @note This macro must be placed at the beginning of a scope (function or block).
 * @note Multiple ZoneScopedN macros in nested scopes create nested zones in Tracy.
 * @note In release builds (NDEBUG defined), this macro expands to nothing, ensuring
 *       zero runtime overhead in production code.
 * @note The name parameter must be a compile-time string literal, not a runtime string.
 *
 * @code
 * #include <debug/tracing/macros.hxx>
 *
 * void ProcessData() {
 *     ZoneScopedN("Data Processing");  // Custom name instead of "ProcessData"
 *     // Function implementation...
 * }
 *
 * void HandleRequest(int type) {
 *     if (type == 1) {
 *         ZoneScopedN("Fast Path");
 *         // Fast path implementation...
 *     } else {
 *         ZoneScopedN("Slow Path");
 *         // Slow path implementation...
 *     }
 * }
 * @endcode
 */
#define ZoneScopedN(name) __tray_zone_scoped_named(name)

#else

/**
 * @brief No-op in release builds
 * @ingroup tracing
 *
 * In release builds (NDEBUG defined), this macro expands to nothing.
 */
#define ZoneScoped

/**
 * @brief No-op in release builds
 * @ingroup tracing
 *
 * In release builds (NDEBUG defined), this macro expands to nothing.
 *
 * @param name Ignored in release builds
 */
#define ZoneScopedN(name)

#endif

#endif  // LOGENIUM_DEBUG_TRACING_MACROS_HXX
