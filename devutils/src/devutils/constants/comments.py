# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from dataclasses import dataclass


@dataclass(frozen=True)
class _Comments:
    c: str = "//"
    cpp: str = c
    python: str = "#"
    cmake: str = python
    powershell: str = "#"
    bat: str = "@REM"


Comments = _Comments()
