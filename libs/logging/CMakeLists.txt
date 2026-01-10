# SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

project(
        logging
        DESCRIPTION "logenium logging library"
        LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/options.cmake")

log_info("Project ${PROJECT_NAME} version ${PROJECT_VERSION}")
log_info("Building in ${CMAKE_BUILD_TYPE} mode")

log_info("Checking provided options")
if(LGENIUM_LOGGING_USE_COLOR_LOGS AND NOT LOGENIUM_LOGGING_USE_FMTLIB)
        log_warning("Color logs are enabled but fmtlib is not enabled, color logs will be disabled")
        set(LOGENIUM_LOGGING_USE_COLOR_LOGS OFF CACHE BOOL "Use color logs (requires fmtlib)" FORCE)
endif()

log_info("Provided options:")
log_info("  LOGENIUM_LOGGING_USE_FMTLIB: ${LOGENIUM_LOGGING_USE_FMTLIB}")
log_info("  LOGENIUM_LOGGING_USE_COLOR_LOGS: ${LOGENIUM_LOGGING_USE_COLOR_LOGS}")

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake")

add_library(
        logging
        src/internal/utils/to_color.cxx
)

add_library(logenium::logging ALIAS logging)

target_include_directories(
        logging
        PUBLIC
        include
)

target_compile_features(
        logging
        PUBLIC
        cxx_std_23
)

target_link_libraries(
        logging
        PUBLIC
        logenium::corelib
        logenium::debug
)

if(LOGENIUM_LOGGING_USE_FMTLIB)
        log_info("Using fmtlib for logging")

        target_link_libraries(
                logging
                PUBLIC
                fmt::fmt
        )

        log_info("Defining __LOGENIUM_LOGGING_USE_FMTLIB__ to 1")
        target_compile_definitions(
                logging
                PUBLIC
                __LOGENIUM_LOGGING_USE_FMTLIB__=1
        )

        if(LOGENIUM_LOGGING_USE_COLOR_LOGS)
                log_info("Using color logs")

                log_info("Defining __LOGENIUM_LOGGING_USE_COLOR_LOGS__ to 1")
                target_compile_definitions(
                        logging
                        PUBLIC
                        __LOGENIUM_LOGGING_USE_COLOR_LOGS__=1
                )
        else()
                log_info("Not using color logs")

                log_info("Defining __LOGENIUM_LOGGING_USE_COLOR_LOGS__ to 0")
                target_compile_definitions(
                        logging
                        PUBLIC
                        __LOGENIUM_LOGGING_USE_COLOR_LOGS__=0
                )
        endif()

else()
        log_info("Not using fmtlib for logging")

        log_info("Defining __LOGENIUM_LOGGING_USE_FMTLIB__ to 0")
        log_info("Defining __LOGENIUM_LOGGING_USE_COLOR_LOGS__ to 0")
        target_compile_definitions(
                logging
                PUBLIC
                __LOGENIUM_LOGGING_USE_FMTLIB__=0
                __LOGENIUM_LOGGING_USE_COLOR_LOGS__=0
        )
endif()
