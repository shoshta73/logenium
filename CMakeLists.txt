# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.31...4.0)

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

add_library(
        logeniumlib

        src/application.cxx
        src/window.cxx

        src/platform/windows/application.cxx
        src/platform/windows/window.cxx

        src/platform/linux/application.cxx
        src/platform/linux/window.cxx

        src/platform/linux/X11/application.cxx
        src/platform/linux/X11/window.cxx

        src/platform/linux/wayland/application.cxx
        src/platform/linux/wayland/window.cxx
)

target_compile_features(
        logeniumlib
        PUBLIC
        cxx_std_23
        c_std_23
)

target_include_directories(
        logeniumlib
        PUBLIC
        include
)

target_include_directories(
        logeniumlib
        PRIVATE
        src
)

target_link_libraries(
        logeniumlib
        PUBLIC
        xheader
        logenium::debug
        corelib
)

target_compile_definitions(
        logeniumlib
        PRIVATE
        __LOGENIUM_SOURCE__
        $<$<PLATFORM_ID:Windows>:_CRT_SECURE_NO_WARNINGS>
)

add_executable(
        logenium

        src/main.cxx
)

target_link_libraries(
        logenium
        PRIVATE
        logeniumlib
)

target_compile_features(
        logenium
        PRIVATE
        cxx_std_23
        c_std_23
)

target_compile_definitions(
        logenium
        PRIVATE
        __LOGENIUM_SOURCE__
        $<$<PLATFORM_ID:Windows>:_CRT_SECURE_NO_WARNINGS>
)

if(LOGENIUM_BUILD_TESTS)
        if(TARGET xheader_tests)
                get_target_property(XHEADER_TESTS_SRCS xheader_tests SOURCES)
        endif()

        if(TARGET debug_tests)
                get_target_property(DEBUG_TESTS_SRCS debug_tests SOURCES)
        endif()

        if(TARGET corelib_tests)
                get_target_property(CORELIB_TESTS_SRCS corelib_tests SOURCES)
        endif()

        add_subdirectory(tests)

        if(TARGET logenium_tests)
                get_target_property(LOGENIUM_TESTS_SRCS logenium_tests SOURCES)
        endif()

        add_executable(
                all_logenium_tests
                ${XHEADER_TESTS_SRCS}
                ${DEBUG_TESTS_SRCS}
                ${CORELIB_TESTS_SRCS}
                ${LOGENIUM_TESTS_SRCS}
        )

        target_link_libraries(
                all_logenium_tests
                PRIVATE
                xheader
                logenium::debug
                corelib
                logeniumlib
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

        target_compile_definitions(
                all_logenium_tests
                PRIVATE
                __LOGENIUM_SOURCE__
        )

        gtest_discover_tests(
                all_logenium_tests
        )
endif()
