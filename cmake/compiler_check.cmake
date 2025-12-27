# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

function(check_compiler_is_clang)
    set(_errors "")

    if(NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
        list(APPEND _errors "C compiler is not Clang (detected: ${CMAKE_C_COMPILER_ID})")
    elseif(NOT CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
        list(APPEND _errors "C compiler is clang-cl, which is not allowed (use clang instead)")
    endif()

    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        list(APPEND _errors "C++ compiler is not Clang (detected: ${CMAKE_CXX_COMPILER_ID})")
    elseif(NOT CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
        list(APPEND _errors "C++ compiler is clang-cl, which is not allowed (use clang++ instead)")
    endif()

    if(_errors)
        message(FATAL_ERROR
            "\n"
            "========================================\n"
            "Compiler Check Failed\n"
            "========================================\n"
            "${_errors}\n"
            "\n"
            "This project requires:\n"
            "  - C compiler: clang\n"
            "  - C++ compiler: clang++\n"
            "\n"
            "Set compilers using:\n"
            "  export CC=clang CXX=clang++\n"
            "  cmake -B build -G Ninja\n"
            "\n"
            "Or:\n"
            "  cmake -B build -G Ninja -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++\n"
            "========================================\n"
        )
    endif()

    log_info("Compiler check passed: clang (C) and clang++ (C++)")
endfunction()

check_compiler_is_clang()
