add_executable(
        xheader_dlfnc_tests
        ${CMAKE_CURRENT_SOURCE_DIR}/dlopen.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/dlclose.cxx
)

target_link_libraries(
        xheader_dlfnc_tests
        PRIVATE
        xheader
        gtest
        gmock
        gtest_main
        gmock_main
)

target_compile_features(
        xheader_dlfnc_tests
        PRIVATE
        cxx_std_23
        c_std_23
)

gtest_discover_tests(
        xheader_dlfnc_tests
)
