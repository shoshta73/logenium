cmake_minimum_required(VERSION 4.0)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(LOGENIUM_IS_TOPLEVEL_PROJECT ON)
else()
        set(LOGENIUM_IS_TOPLEVEL_PROJECT OFF)
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

add_executable(
        logenium

        src/main.cxx
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
