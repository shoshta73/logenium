# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

add_executable(
        corelib_auto_release_tests

        ${CMAKE_CURRENT_SOURCE_DIR}/auto_release.cxx
)

target_link_libraries(
        corelib_auto_release_tests
        PRIVATE
        logenium::corelib
        gtest
        gmock
        gtest_main
        gmock_main
)

target_compile_features(
        corelib_auto_release_tests
        PRIVATE
        cxx_std_23
)

gtest_discover_tests(
        corelib_auto_release_tests
)
