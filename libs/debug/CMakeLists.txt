# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

project(
        debug
        VERSION 0.1.0
        LANGUAGES CXX
        DESCRIPTION "Debug library for logenium"
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/options.cmake")

log_info("Project ${PROJECT_NAME} version ${PROJECT_VERSION}")
log_info("Building in ${CMAKE_BUILD_TYPE} mode")

log_info("Checking provided options")
if(LOGENIUM_DEBUG_USE_COLOR_LOGS AND NOT LOGENIUM_DEBUG_USE_FMTLIB)
        log_warning("Color logs are enabled but fmtlib is not enabled, color logs will be disabled")
        set(LOGENIUM_DEBUG_USE_COLOR_LOGS OFF CACHE BOOL "Use color logs (requires fmtlib)" FORCE)
endif()

log_info("Provided options:")
log_info("  LOGENIUM_DEBUG_USE_FAST_STACKTRACE: ${LOGENIUM_DEBUG_USE_FAST_STACKTRACE}")
log_info("  LOGENIUM_DEBUG_USE_FMTLIB: ${LOGENIUM_DEBUG_USE_FMTLIB}")
log_info("  LOGENIUM_DEBUG_USE_COLOR_LOGS: ${LOGENIUM_DEBUG_USE_COLOR_LOGS}")

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake")

add_library(
        debug

        src/breakpoint_if_debugging.cxx
        src/breakpoint.cxx
        src/is_debugger_present.cxx
)

add_library(
        logenium::debug ALIAS debug
)

target_include_directories(
        debug
        PUBLIC
        include
)

target_compile_features(
        debug
        PUBLIC
        cxx_std_23
)

target_link_libraries(
        debug
        PUBLIC
        xheader
        $<$<PLATFORM_ID:Linux>:stdc++exp>
)

if (LOGENIUM_DEBUG_USE_FAST_STACKTRACE)
        log_info("Using fast stacktrace (reversed printing in assert)")

        log_info("Defining __LOGENIUM_DEBUG_USE_FAST_STACKTRACE__ to 1")
        target_compile_definitions(
                debug
                PUBLIC
                __LOGENIUM_DEBUG_USE_FAST_STACKTRACE__=1
        )
else()
        log_info("Not using fast stacktrace (not reversed printing in assert)")

        log_info("Defining __LOGENIUM_DEBUG_USE_FAST_STACKTRACE__ to 0")
        target_compile_definitions(
                debug
                PUBLIC
                __LOGENIUM_DEBUG_USE_FAST_STACKTRACE__=0
        )
endif()

if (LOGENIUM_DEBUG_USE_FMTLIB)
        log_info("Using fmtlib for debug logging")
        target_link_libraries(
                debug
                PUBLIC
                fmt::fmt
        )

        log_info("Defining __LOGENIUM_DEBUG_USE_FMTLIB__ to 1")
        target_compile_definitions(
                debug
                PUBLIC
                __LOGENIUM_DEBUG_USE_FMTLIB__=1
        )

        if(LOGENIUM_DEBUG_USE_COLOR_LOGS)
                log_info("Using color logs")

                log_info("Defining __LOGENIUM_DEBUG_USE_COLOR_LOGS__ to 1")
                target_compile_definitions(
                        debug
                        PUBLIC
                        __LOGENIUM_DEBUG_USE_COLOR_LOGS__=1
                )
        else()
                log_info("Not using color logs")

                log_info("Defining __LOGENIUM_DEBUG_USE_COLOR_LOGS__ to 0")
                target_compile_definitions(
                        debug
                        PUBLIC
                        __LOGENIUM_DEBUG_USE_COLOR_LOGS__=0
                )
        endif()

else()
        log_info("Not using fmtlib for debug logging")

        log_info("Defining __LOGENIUM_DEBUG_USE_FMTLIB__ to 0")
        target_compile_definitions(
                debug
                PUBLIC
                __LOGENIUM_DEBUG_USE_FMTLIB__=0
        )

        log_info("Defining __LOGENIUM_DEBUG_USE_COLOR_LOGS__ to 0")
        target_compile_definitions(
                debug
                PUBLIC
                __LOGENIUM_DEBUG_USE_COLOR_LOGS__=0
        )
endif()

if(LOGENIUM_BUILD_TESTS AND LOGENIUM_DEBUG_BUILD_TESTS)
        log_info("Building debug tests")
        add_subdirectory(tests)
endif()
