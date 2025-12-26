add_executable(
        xheader_xcb_xproto_h_tests
        ${CMAKE_CURRENT_SOURCE_DIR}/xcb_create_window.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/xcb_destroy_window.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/xcb_map_window.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/xcb_setup_roots_iterator.cxx
)

target_link_libraries(
        xheader_xcb_xproto_h_tests
        PRIVATE
        xheader
        gtest
        gmock
        gtest_main
        gmock_main
)

target_compile_features(
        xheader_xcb_xproto_h_tests
        PRIVATE
        cxx_std_23
        c_std_23
)

gtest_discover_tests(
        xheader_xcb_xproto_h_tests
)
