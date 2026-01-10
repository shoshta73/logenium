# SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

project(
        logging
        VERSION 0.2.0
        DESCRIPTION "logenium logging library"
        LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/options.cmake")

log_info("Project ${PROJECT_NAME} version ${PROJECT_VERSION}")
log_info("Building in ${CMAKE_BUILD_TYPE} mode")

log_info("Provided options:")
log_info("  LOGENIUM_LOGGING_USE_FMTLIB: ${LOGENIUM_LOGGING_USE_FMTLIB}")

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake")

add_library(
        logging
        INTERFACE
        include/logging/logging.hxx
)

add_library(logenium::logging ALIAS logging)

target_include_directories(
        logging
        INTERFACE
        include
)

target_compile_features(
        logging
        INTERFACE
        cxx_std_23
)

if(LOGENIUM_LOGGING_USE_FMTLIB)
        log_info("Using fmtlib for logging")

        target_link_libraries(
                logging
                INTERFACE
                fmt::fmt
        )

        log_info("Defining __LOGENIUM_LOGGING_USE_FMTLIB__ to 1")
        target_compile_definitions(
                logging
                INTERFACE
                __LOGENIUM_LOGGING_USE_FMTLIB__=1
        )

else()
        log_info("Not using fmtlib for logging")

        log_info("Defining __LOGENIUM_LOGGING_USE_FMTLIB__ to 0")
        target_compile_definitions(
                logging
                INTERFACE
                __LOGENIUM_LOGGING_USE_FMTLIB__=0
        )
endif()
