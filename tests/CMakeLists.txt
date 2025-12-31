# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

file(GLOB_RECURSE LOGENIUM_TESTS_SOURCES CONFIGURE_DEPENDS "*.cxx")

add_executable(
        logenium_tests
        ${LOGENIUM_TESTS_SOURCES}
)

target_link_libraries(
        logenium_tests
        PRIVATE
        logeniumlib
        gtest
        gmock
        gtest_main
        gmock_main
)

target_compile_features(
        logenium_tests
        PRIVATE
        cxx_std_23
)

target_compile_definitions(
        logenium_tests
        PRIVATE
        __LOGENIUM_SOURCE__
)

gtest_discover_tests(
        logenium_tests
)
