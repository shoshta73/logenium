# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from typing import ClassVar


class Comments:
    c: ClassVar[str] = "//"
    cpp: ClassVar[str] = c
    python: ClassVar[str] = "#"
    cmake: ClassVar[str] = python
    powershell: ClassVar[str] = "#"
    bat: ClassVar[str] = "@REM"
    bash: ClassVar[str] = "#"
