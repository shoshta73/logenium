# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

add_executable(
        xheader_xdg_shell_tests
        ${CMAKE_CURRENT_SOURCE_DIR}/xdg_wm_base_get_xdg_surface.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/xdg_surface_get_toplevel.cxx
)

target_link_libraries(
        xheader_xdg_shell_tests
        PRIVATE
        xheader
        gtest
        gmock
        gtest_main
        gmock_main
)

target_compile_features(
        xheader_xdg_shell_tests
        PRIVATE
        cxx_std_23
        c_std_23
)

gtest_discover_tests(
        xheader_xdg_shell_tests
)
