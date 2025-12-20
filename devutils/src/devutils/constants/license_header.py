from dataclasses import dataclass, field

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

_licence_header_bat: list[str] = []

for line in _LICENCE_HEADER:
    _licence_header_bat.append(" ".join([Comments.bat, line]) + "\n")
_licence_header_bat.append("\n")


@dataclass(frozen=True)
class _LicenseHeaders:
    c: list[str] = field(default_factory=lambda: _licence_header_c.copy())
    cpp: list[str] = field(default_factory=lambda: _licence_header_cpp.copy())
    python: list[str] = field(default_factory=lambda: _licence_header_python.copy())
    cmake: list[str] = field(default_factory=lambda: _licence_header_cmake.copy())
    powershell: list[str] = field(default_factory=lambda: _license_header_powershell.copy())
    bat: list[str] = field(default_factory=lambda: _licence_header_bat.copy())


LicenseHeaders = _LicenseHeaders()
