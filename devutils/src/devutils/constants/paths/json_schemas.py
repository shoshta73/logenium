# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from dataclasses import dataclass
from pathlib import Path

from .directories import Directories as _Directories


@dataclass(frozen=True)
class JsonSchemas:
    codegen: Path = _Directories.devutils_root / "data" / "schemas" / "r1" / "codegen.schema.json"
    config_r1: Path = _Directories.devutils_root / "data" / "schemas" / "r1" / "config.schema.json"
    config_r2: Path = _Directories.devutils_root / "data" / "schemas" / "r2" / "config.schema.json"
    config: Path = config_r2
