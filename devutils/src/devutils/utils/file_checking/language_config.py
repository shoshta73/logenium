# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import pathlib
from dataclasses import dataclass

from .utils import collect_files as _collect_files


@dataclass
class LanguageConfig:
    name: str
    extensions: list[str]
    search_dirs: list[pathlib.Path]
    specific_files: list[pathlib.Path]

    def collect_files(self) -> list[pathlib.Path]:
        return _collect_files(self.extensions, self.search_dirs, self.specific_files)
