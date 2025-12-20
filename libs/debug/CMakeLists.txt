# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

project(
        debug
        VERSION 0.0.1
        LANGUAGES CXX
        DESCRIPTION "Debug library for logenium"
)

log_info("Project ${PROJECT_NAME} version ${PROJECT_VERSION}")
log_info("Building in ${CMAKE_BUILD_TYPE} mode")

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
)
