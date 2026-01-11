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

log_info("Provided options:")
log_info("  LOGENIUM_LOGGING_BUILD_TESTS: ${LOGENIUM_LOGGING_BUILD_TESTS}")

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
        fmt::fmt
)

if(LOGENIUM_BUILD_TESTS AND LOGENIUM_LOGGING_BUILD_TESTS)
        log_info("Building logging tests")
        add_subdirectory(tests)
endif()
