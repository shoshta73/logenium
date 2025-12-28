# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

from dataclasses import dataclass
from pathlib import Path

from .directories import Directories as _Directories


@dataclass(frozen=True)
class JsonSchemas:
    codegen: Path = _Directories.devutils_root / "data" / "schemas" / "codegen.schema.json"
    config: Path = _Directories.devutils_root / "data" / "schemas" / "config.schema.json"
