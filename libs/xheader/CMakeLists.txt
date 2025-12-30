# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

# CXX is used for compiling tests
project(
        xheader
        VERSION 0.0.1
        DESCRIPTION "Cross-platform platform specific headers"
        LANGUAGES CXX C
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/options.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake")

log_info("Project ${PROJECT_NAME} version ${PROJECT_VERSION}")
log_info("Building in ${CMAKE_BUILD_TYPE} mode")

add_library(
        xheader

        src/windows/create_window_ex_a.c
        src/windows/create_window_ex_w.c
        src/windows/debug_break.c
        src/windows/def_window_proc_a.c
        src/windows/def_window_proc_w.c
        src/windows/destroy_window.c
        src/windows/dispatch_message_a.c
        src/windows/dispatch_message_w.c
        src/windows/get_message_a.c
        src/windows/get_message_w.c
        src/windows/get_module_handle_a.c
        src/windows/get_module_handle_w.c
        src/windows/get_window_long_ptr_a.c
        src/windows/get_window_long_ptr_w.c
        src/windows/is_debugger_present.c
        src/windows/load_cursor_a.c
        src/windows/load_cursor_w.c
        src/windows/load_icon_a.c
        src/windows/load_icon_w.c
        src/windows/post_quit_message.c
        src/windows/register_class_ex_a.c
        src/windows/register_class_ex_w.c
        src/windows/set_window_long_ptr_a.c
        src/windows/set_window_long_ptr_w.c
        src/windows/show_window.c
        src/windows/translate_message.c
        src/windows/unregister_class_a.c
        src/windows/unregister_class_w.c
        src/windows/update_window.c

        src/dlfcn/dlopen.c
        src/dlfcn/dlclose.c

        src/sys/mman/memfd_create.c
        src/sys/mman/mmap.c
        src/sys/mman/munmap.c

        src/unistd/ftruncate.c
        src/unistd/close.c

        src/xcb/xcb/xcb_connect.c
        src/xcb/xcb/xcb_connection_has_error.c
        src/xcb/xcb/xcb_disconnect.c
        src/xcb/xcb/xcb_flush.c
        src/xcb/xcb/xcb_generate_id.c
        src/xcb/xcb/xcb_get_setup.c
        src/xcb/xcb/xcb_wait_for_event.c
        src/xcb/xproto/xcb_create_window.c
        src/xcb/xproto/xcb_destroy_window.c
        src/xcb/xproto/xcb_map_window.c
        src/xcb/xproto/xcb_setup_roots_iterator.c

        src/internal/xdg-shell-protocol.c
        src/internal/xdg-decoration-unstable-v1-protocol.c

        src/wayland-client-core/wl_display_connect.c
        src/wayland-client-core/wl_display_roundtrip.c
        src/wayland-client-core/wl_display_disconnect.c
        src/wayland-client-core/wl_display_dispatch.c

        src/wayland-client-protocol/wl_registry_interface.c
        src/wayland-client-protocol/wl_compositor_interface.c
        src/wayland-client-protocol/wl_shm_interface.c
        src/xdg-shell/xdg_wm_base_interface.c
        src/xdg-decoration/zxdg_decoration_manager_v1_interface.c
)

add_library(
        logenium::xheader ALIAS xheader
)

target_include_directories(
        xheader
        PUBLIC
        include
)

target_compile_features(
        xheader
        PRIVATE
        c_std_23
)

target_link_libraries(
        xheader
        PUBLIC
        $<$<PLATFORM_ID:Linux>:dl>
        $<$<PLATFORM_ID:Linux>:xcb>
        $<$<PLATFORM_ID:Linux>:wayland-client>
)

if(LOGENIUM_XHEADER_BUILD_TESTS AND LOGENIUM_BUILD_TESTS)
        log_info("Building xheader tests")
        add_subdirectory(tests)
endif()
