# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from dataclasses import dataclass, field

from _typeshed import Incomplete

from .comments import Comments as Comments

@dataclass(frozen=True)
class _LicenseHeaders:
    c: list[str] = field(default_factory=Incomplete)
    cpp: list[str] = field(default_factory=Incomplete)
    python: list[str] = field(default_factory=Incomplete)
    cmake: list[str] = field(default_factory=Incomplete)
    powershell: list[str] = field(default_factory=Incomplete)
    bat: list[str] = field(default_factory=Incomplete)
    bash: list[str] = field(default_factory=Incomplete)

LicenseHeaders: Incomplete
