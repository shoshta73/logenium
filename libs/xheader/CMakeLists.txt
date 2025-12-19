project(
        xheader
        VERSION 0.0.1
        DESCRIPTION "Cross-platform platform specific headers"
        LANGUAGES C
)

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
