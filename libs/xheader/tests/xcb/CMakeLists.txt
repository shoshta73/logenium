# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(xcb)
add_subdirectory(xproto)

get_target_property(XHEADER_XCB_XCB_H_SRCS xheader_xcb_xcb_h_tests SOURCES)
get_target_property(XHEADER_XCB_XPROTO_H_SRCS xheader_xcb_xproto_h_tests SOURCES)

add_executable(
        xheader_xcb_tests
        ${XHEADER_XCB_XCB_H_SRCS}
        ${XHEADER_XCB_XPROTO_H_SRCS}
)

target_link_libraries(
        xheader_xcb_tests
        PRIVATE
        xheader
        gtest
        gmock
        gtest_main
        gmock_main
)

target_compile_features(
        xheader_xcb_tests
        PRIVATE
        cxx_std_23
        c_std_23
)

gtest_discover_tests(
        xheader_xcb_tests
)
