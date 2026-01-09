# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from dataclasses import dataclass
from pathlib import Path

from .directories import Directories as _Directories


@dataclass(frozen=True)
class ConfigFiles:
    config: Path = _Directories.root / "config.yaml"
    xheader_wayland_codegen: Path = _Directories.xheader_root / "data" / "codegen" / "wayland.yaml"
    vscode_bookmarks: Path = _Directories.vscode / "bookmarks.json"
