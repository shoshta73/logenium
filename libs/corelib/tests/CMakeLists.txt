# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(utility)

get_target_property(CORELIB_UTILITY_SRCS corelib_utility_tests SOURCES)

add_executable(
        corelib_tests
        ${CORELIB_UTILITY_SRCS}
)

target_link_libraries(
        corelib_tests
        PRIVATE
        logenium::corelib
        gtest
        gmock
        gtest_main
        gmock_main
)

target_compile_features(
        corelib_tests
        PRIVATE
        cxx_std_23
)

gtest_discover_tests(
        corelib_tests
)
