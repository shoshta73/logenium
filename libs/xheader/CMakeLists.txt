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

        src/windows/get_module_handle_w.c
        src/windows/get_module_handle_a.c
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
