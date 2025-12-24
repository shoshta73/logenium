# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

add_executable(
        debug_utility_tests

        ${CMAKE_CURRENT_SOURCE_DIR}/assert.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/breakpoint.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/breakpoint_if_debugging.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/is_debugger_present.cxx
)

target_link_libraries(
        debug_utility_tests
        PRIVATE
        logenium::debug
        gtest
        gmock
        gtest_main
        gmock_main
)

target_compile_features(
        debug_utility_tests
        PRIVATE
        cxx_std_23
)

gtest_discover_tests(
        debug_utility_tests
)

get_target_property(DEBUG_UTILITY_SRCS debug_utility_tests SOURCES)

add_executable(
        debug_tests
        ${DEBUG_UTILITY_SRCS}
)

target_link_libraries(
        debug_tests
        PRIVATE
        logenium::debug
        gtest
        gmock
        gtest_main
        gmock_main
)

target_compile_features(
        debug_tests
        PRIVATE
        cxx_std_23
)

gtest_discover_tests(
        debug_tests
)
