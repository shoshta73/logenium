# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import pathlib
from dataclasses import dataclass

from .file_status import FileStatus


@dataclass
class FileResult:
    path: pathlib.Path
    status: FileStatus
    error: str | None = None
