# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from typing import ClassVar

from .comments import Comments as Comments

class LicenseHeaders:
    c: ClassVar[list[str]]
    cpp: ClassVar[list[str]]
    python: ClassVar[list[str]]
    cmake: ClassVar[list[str]]
    powershell: ClassVar[list[str]]
    bat: ClassVar[list[str]]
    bash: ClassVar[list[str]]
