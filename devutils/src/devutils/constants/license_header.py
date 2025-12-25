# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from typing import ClassVar

from .comments import Comments

_LICENCE_HEADER: list[str] = [
    "SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors",
    "SPDX-License-Identifier: BSD-3-Clause",
]

_licence_header_c: list[str] = []

for line in _LICENCE_HEADER:
    _licence_header_c.append(" ".join([Comments.c, line]) + "\n")
_licence_header_c.append("\n")

_licence_header_cpp = _licence_header_c

_licence_header_python: list[str] = []

for line in _LICENCE_HEADER:
    _licence_header_python.append(" ".join([Comments.python, line]) + "\n")
_licence_header_python.append("\n")

_licence_header_cmake: list[str] = _licence_header_python
_license_header_powershell: list[str] = _licence_header_python
_license_header_bash: list[str] = _licence_header_python

_licence_header_bat: list[str] = []

for line in _LICENCE_HEADER:
    _licence_header_bat.append(" ".join([Comments.bat, line]) + "\n")
_licence_header_bat.append("\n")


class LicenseHeaders:
    c: ClassVar[list[str]] = _licence_header_c
    cpp: ClassVar[list[str]] = _licence_header_cpp
    python: ClassVar[list[str]] = _licence_header_python
    cmake: ClassVar[list[str]] = _licence_header_cmake
    powershell: ClassVar[list[str]] = _license_header_powershell
    bat: ClassVar[list[str]] = _licence_header_bat
    bash: ClassVar[list[str]] = _license_header_bash
