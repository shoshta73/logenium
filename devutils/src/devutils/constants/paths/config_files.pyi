# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from dataclasses import dataclass
from pathlib import Path

@dataclass(frozen=True)
class ConfigFiles:
    config: Path = ...
    xheader_wayland_codegen: Path = ...
