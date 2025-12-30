# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause
file(GLOB_RECURSE CORELIB_TESTS_SOURCES CONFIGURE_DEPENDS *.cxx)

add_executable(
        corelib_tests
        ${CORELIB_TESTS_SOURCES}
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
