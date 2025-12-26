# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

add_executable(
        xheader_wayland_util_tests
        ${CMAKE_CURRENT_SOURCE_DIR}/wl_array.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/wl_interface.cxx
)

target_link_libraries(
        xheader_wayland_util_tests
        PRIVATE
        xheader
        gtest
        gmock
        gtest_main
        gmock_main
)

target_compile_features(
        xheader_wayland_util_tests
        PRIVATE
        cxx_std_23
        c_std_23
)

gtest_discover_tests(
        xheader_wayland_util_tests
)
