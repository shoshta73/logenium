add_executable(
        xheader_xcb_xcb_h_tests
        ${CMAKE_CURRENT_SOURCE_DIR}/xcb_connect.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/xcb_connection_has_error.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/xcb_disconnect.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/xcb_flush.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/xcb_generate_id.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/xcb_get_setup.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/xcb_wait_for_event.cxx
)

target_link_libraries(
        xheader_xcb_xcb_h_tests
        PRIVATE
        xheader
        gtest
        gmock
        gtest_main
        gmock_main
)

target_compile_features(
        xheader_xcb_xcb_h_tests
        PRIVATE
        cxx_std_23
        c_std_23
)

gtest_discover_tests(
        xheader_xcb_xcb_h_tests
)
