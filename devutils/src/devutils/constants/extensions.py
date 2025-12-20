# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from dataclasses import dataclass, field


@dataclass(frozen=True)
class _Extensions:
    c_source: list[str] = field(default_factory=lambda: [".c", ".h"])
    cpp_source: list[str] = field(default_factory=lambda: [".cxx", ".hxx"])
    cmake_source: list[str] = field(default_factory=lambda: [".cmake"])
    python_source: list[str] = field(default_factory=lambda: [".py", ".pyi"])
    powershell_source: list[str] = field(default_factory=lambda: [".ps1"])
    bat_source: list[str] = field(default_factory=lambda: [".bat"])


Extensions = _Extensions()
