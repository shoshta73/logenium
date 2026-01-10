# SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

file(GLOB_RECURSE LOGGING_TESTS_SOURCES CONFIGURE_DEPENDS *.cxx)

add_executable(
        logging_tests
        ${LOGGING_TESTS_SOURCES}
)

target_link_libraries(
        logging_tests
        PRIVATE
        logenium::logging
        gtest
        gmock
        gtest_main
        gmock_main
)

target_compile_features(
        logging_tests
        PRIVATE
        cxx_std_23
)

gtest_discover_tests(
        logging_tests
)
