# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

project(
        corelib
        VERSION 0.0.1
        DESCRIPTION "Core library for Logenium"
        LANGUAGES CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

log_info("Project ${PROJECT_NAME} version ${PROJECT_VERSION}")
log_info("Building in ${CMAKE_BUILD_TYPE} mode")

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/options.cmake")

add_library(
        corelib

        src/utility/type_name.cxx
)

add_library(
        logenium::corelib ALIAS corelib
)

target_include_directories(
        corelib
        PUBLIC
        include
)

target_compile_features(
        corelib
        PUBLIC
        cxx_std_23
)

target_link_libraries(
        corelib
        PUBLIC
        logenium::debug
)

if(LOGENIUM_BUILD_TESTS AND LOGENIUM_CORELIB_BUILD_TESTS)
        log_info("Building corelib tests")
        add_subdirectory(tests)
endif()
