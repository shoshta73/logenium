# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from .comments import Comments


def generate_license_header(year: int, comment_style: str) -> list[str]:
    lines = [
        f"SPDX-FileCopyrightText: {year} Logenium Authors and Contributors",
        "SPDX-License-Identifier: BSD-3-Clause",
    ]

    header: list[str] = []
    for line in lines:
        header.append(" ".join([comment_style, line]) + "\n")
    header.append("\n")

    return header


def generate_c_header(year: int) -> list[str]:
    return generate_license_header(year, Comments.c)


def generate_cpp_header(year: int) -> list[str]:
    return generate_license_header(year, Comments.cpp)


def generate_python_header(year: int) -> list[str]:
    return generate_license_header(year, Comments.python)


def generate_cmake_header(year: int) -> list[str]:
    return generate_license_header(year, Comments.cmake)


def generate_powershell_header(year: int) -> list[str]:
    return generate_license_header(year, Comments.powershell)


def generate_bat_header(year: int) -> list[str]:
    return generate_license_header(year, Comments.bat)


def generate_bash_header(year: int) -> list[str]:
    return generate_license_header(year, Comments.bash)
