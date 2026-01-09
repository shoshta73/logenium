# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

log_info("Declaring googletest (v1.17.0, shallow, progress)")
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
        GIT_SHALLOW    ON
        GIT_PROGRESS   ON
)

log_info("Declaring fmtlib (v12.1.0, shallow, progress)")
FetchContent_Declare(
        fmtlib
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        12.1.0
        GIT_SHALLOW    ON
        GIT_PROGRESS   ON
)

log_info("Declaring tracy (v0.13.0, shallow, progress)")
FetchContent_Declare(
        tracy
        GIT_REPOSITORY https://github.com/wolfpld/tracy.git
        GIT_TAG        v0.13.0
        GIT_SHALLOW    ON
        GIT_PROGRESS   ON
)

log_info("Fetching dependencies")
log_info("Fetching googletest (v1.17.0, shallow, progress)")
FetchContent_MakeAvailable(googletest)

if(LOGENIUM_DEBUG_USE_FMTLIB)
        log_info("Fetching fmtlib (v12.1.0, shallow, progress)")
        FetchContent_MakeAvailable(fmtlib)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR "MinSizeRel")
        log_info("Disabling tracy (not in debug mode)")
        set(TRACY_ENABLE OFF)
endif()

log_info("Fetching tracy (v0.13.0, shallow, progress)")
FetchContent_MakeAvailable(tracy)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
