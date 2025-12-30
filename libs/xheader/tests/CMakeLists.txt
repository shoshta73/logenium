# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

file(GLOB_RECURSE XHEADER_TESTS_SOURCES CONFIGURE_DEPENDS *.cxx)

add_executable(
        xheader_tests
        ${XHEADER_TESTS_SOURCES}
)

target_link_libraries(
        xheader_tests
        PRIVATE
        xheader
        gtest
        gmock
        gtest_main
        gmock_main
)

target_compile_features(
        xheader_tests
        PRIVATE
        cxx_std_23
        c_std_23
)

gtest_discover_tests(
        xheader_tests
)
