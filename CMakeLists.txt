# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 4.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(logging)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(LOGENIUM_IS_TOPLEVEL_PROJECT ON)
        log_info("Building logenium as top-level project")
else()
        set(LOGENIUM_IS_TOPLEVEL_PROJECT OFF)
        log_info("Building logenium as subproject")
endif()

if(LOGENIUM_IS_TOPLEVEL_PROJECT)
        set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

project(
        logenium
        VERSION 0.0.1
        DESCRIPTION "Element of logic"
        LANGUAGES CXX C
)

include(compiler_check)
include(options)
include(dependencies)

if(LOGENIUM_BUILD_TESTS)
        log_info("enabling testing")
        enable_testing()
        include(CTest)
        include(GoogleTest)
endif()

add_subdirectory(libs)

log_info("Project ${PROJECT_NAME} version ${PROJECT_VERSION}")
log_info("Building in ${CMAKE_BUILD_TYPE} mode")

add_executable(
        logenium

        src/main.cxx

        src/application.cxx
        src/window.cxx

        src/platform/windows/application.cxx
        src/platform/windows/window.cxx
)

target_compile_features(
        logenium
        PRIVATE
        cxx_std_23
        c_std_23
)

target_include_directories(
        logenium
        PRIVATE
        src
        include
)

target_link_libraries(
        logenium
        PRIVATE
        xheader
        logenium::debug
)

target_compile_definitions(
        logenium
        PRIVATE
        __LOGENIUM_SOURCE__
)

if(LOGENIUM_BUILD_TESTS)
        if(TARGET xheader_tests)
                get_target_property(XHEADER_TESTS_SRCS xheader_tests SOURCES)
        endif()

        if(TARGET debug_tests)
                get_target_property(DEBUG_TESTS_SRCS debug_tests SOURCES)
        endif()

        add_executable(
                all_logenium_tests
                ${XHEADER_TESTS_SRCS}
                ${DEBUG_TESTS_SRCS}
        )

        target_link_libraries(
                all_logenium_tests
                PRIVATE
                xheader
                logenium::debug
                gtest
                gmock
                gtest_main
                gmock_main
        )

        target_compile_features(
                all_logenium_tests
                PRIVATE
                cxx_std_23
                c_std_23
        )

        gtest_discover_tests(
                all_logenium_tests
        )
endif()
