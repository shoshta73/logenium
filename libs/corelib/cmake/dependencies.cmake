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

log_info("Fetching dependencies")
log_info("Fetching googletest (v1.17.0, shallow, progress)")
FetchContent_MakeAvailable(googletest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
