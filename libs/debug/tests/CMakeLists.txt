# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

file(GLOB_RECURSE DEBUG_TESTS_SOURCES CONFIGURE_DEPENDS *.cxx)

add_executable(
        debug_tests
        ${DEBUG_TESTS_SOURCES}
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
