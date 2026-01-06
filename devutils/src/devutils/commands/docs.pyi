# SPDX-FileCopyrightText: 2026 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import threading
from pathlib import Path

import typer

from devutils.constants.paths.files import DOXYGEN_CONFIGS as DOXYGEN_CONFIGS
from devutils.utils.filesystem import find_files_by_extensions as find_files_by_extensions

docs: typer.Typer

def check_doxygen_is_available() -> bool: ...
def build_single_project(
    project_name: str, config_path: Path, ci: bool, _unused_thread_id: int, output_lock: threading.Lock
) -> bool: ...
def build(ci: bool = ...) -> None: ...
def main(ctx: typer.Context) -> None: ...
