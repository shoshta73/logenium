cmake_minimum_required(VERSION 4.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(logging)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(LOGENIUM_IS_TOPLEVEL_PROJECT ON)
        log_info("Building logenium as top-level project")
else()
        set(LOGENIUM_IS_TOPLEVEL_PROJECT OFF)
        log_info("Building logenium as subproject")
endif()

if(LOGENIUM_IS_TOPLEVEL_PROJECT)
        set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

project(
        logenium
        VERSION 0.0.1
        DESCRIPTION "Element of logic"
        LANGUAGES CXX C
)

add_subdirectory(libs)

log_info("Project ${PROJECT_NAME} version ${PROJECT_VERSION}")
log_info("Building in ${CMAKE_BUILD_TYPE} mode")

add_executable(
        logenium

        src/main.cxx

        src/application.cxx
        src/window.cxx

        src/platform/windows/application.cxx
        src/platform/windows/window.cxx
)

target_compile_features(
        logenium
        PRIVATE
        cxx_std_23
        c_std_23
)

target_include_directories(
        logenium
        PRIVATE
        src
        include
)

target_link_libraries(
        logenium
        PRIVATE
        xheader
        logenium::debug
)

target_compile_definitions(
        logenium
        PRIVATE
        __LOGENIUM_SOURCE__
)
