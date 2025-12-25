# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from typing import ClassVar


class Extensions:
    c_source: ClassVar[list[str]] = [".c", ".h"]
    cpp_source: ClassVar[list[str]] = [".cxx", ".hxx"]
    cmake_source: ClassVar[list[str]] = [".cmake"]
    python_source: ClassVar[list[str]] = [".py", ".pyi"]
    powershell_source: ClassVar[list[str]] = [".ps1"]
    bat_source: ClassVar[list[str]] = [".bat"]
    bash_source: ClassVar[list[str]] = [".sh"]
