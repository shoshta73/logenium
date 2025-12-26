# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

add_subdirectory(windows)
add_subdirectory(dlfcn)
add_subdirectory(xcb)
add_subdirectory(wayland-util)
add_subdirectory(wayland-client-core)
add_subdirectory(wayland-client-protocol)
add_subdirectory(xdg-shell)
add_subdirectory(xdg-decoration)

get_target_property(XHEADER_WIN32API_SRCS xheader_win32api_tests SOURCES)
get_target_property(XHEADER_DLFNC_SRCS xheader_dlfnc_tests SOURCES)
get_target_property(XHEADER_XCB_SRCS xheader_xcb_tests SOURCES)
get_target_property(XHEADER_WAYLAND_UTIL_SRCS xheader_wayland_util_tests SOURCES)
get_target_property(XHEADER_WAYLAND_CLIENT_CORE_SRCS xheader_wayland_client_core_tests SOURCES)
get_target_property(XHEADER_WAYLAND_CLIENT_PROTOCOL_SRCS xheader_wayland_client_protocol_tests SOURCES)
get_target_property(XHEADER_XDG_SHELL_SRCS xheader_xdg_shell_tests SOURCES)
get_target_property(XHEADER_XDG_DECORATION_SRCS xheader_xdg_decoration_tests SOURCES)

add_executable(
        xheader_tests
        ${XHEADER_WIN32API_SRCS}
        ${XHEADER_DLFNC_SRCS}
        ${XHEADER_XCB_SRCS}
        ${XHEADER_WAYLAND_UTIL_SRCS}
        ${XHEADER_WAYLAND_CLIENT_CORE_SRCS}
        ${XHEADER_WAYLAND_CLIENT_PROTOCOL_SRCS}
        ${XHEADER_XDG_SHELL_SRCS}
        ${XHEADER_XDG_DECORATION_SRCS}
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
