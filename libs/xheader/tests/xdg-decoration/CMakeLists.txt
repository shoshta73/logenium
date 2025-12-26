# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

add_executable(
        xheader_xdg_decoration_tests
        ${CMAKE_CURRENT_SOURCE_DIR}/zxdg_decoration_manager_v1_get_toplevel_decoration.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/zxdg_toplevel_decoration_v1_mode_enum.cxx
)

target_link_libraries(
        xheader_xdg_decoration_tests
        PRIVATE
        xheader
        gtest
        gmock
        gtest_main
        gmock_main
)

target_compile_features(
        xheader_xdg_decoration_tests
        PRIVATE
        cxx_std_23
        c_std_23
)

gtest_discover_tests(
        xheader_xdg_decoration_tests
)
