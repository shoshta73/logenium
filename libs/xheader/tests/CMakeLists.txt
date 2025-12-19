add_executable(
        xheader_win32api_tests

        ${CMAKE_CURRENT_SOURCE_DIR}/windows/create_window_ex_a.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/create_window_ex_w.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/debug_break.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/def_window_proc_a.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/def_window_proc_w.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/destroy_window.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/dispatch_message_a.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/dispatch_message_w.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/get_message_a.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/get_message_w.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/get_module_handle_a.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/get_module_handle_w.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/get_window_long_ptr_a.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/get_window_long_ptr_w.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/is_debugger_present.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/load_cursor_a.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/load_cursor_w.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/load_icon_a.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/load_icon_w.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/post_quit_message.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/register_class_ex_a.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/register_class_ex_w.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/set_window_long_ptr_a.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/set_window_long_ptr_w.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/show_window.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/translate_message.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/unregister_class_a.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/unregister_class_w.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/windows/update_window.cxx
)

target_link_libraries(
        xheader_win32api_tests
        PRIVATE
        xheader
        gtest
        gmock
        gtest_main
        gmock_main
)

target_compile_features(
        xheader_win32api_tests
        PRIVATE
        cxx_std_23
        c_std_23
)

gtest_discover_tests(
        xheader_win32api_tests
)

get_target_property(XHEADER_WIN32API_SRCS xheader_win32api_tests SOURCES)

add_executable(
        xheader_tests
        ${XHEADER_WIN32API_SRCS}
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
