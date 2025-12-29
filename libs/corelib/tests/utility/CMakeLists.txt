add_subdirectory(type_name)

get_target_property(CORELIB_UTILITY_TYPE_NAME_SRCS corelib_type_name_tests SOURCES)

add_executable(
        corelib_utility_tests
        ${CORELIB_UTILITY_TYPE_NAME_SRCS}
)

target_link_libraries(
        corelib_utility_tests
        PRIVATE
        logenium::corelib
        gtest
        gmock
        gtest_main
        gmock_main
)

target_compile_features(
        corelib_utility_tests
        PRIVATE
        cxx_std_23
)

gtest_discover_tests(
        corelib_utility_tests
)
