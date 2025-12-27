# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import pathlib
from dataclasses import dataclass

import typer

from devutils.constants import Extensions as Extensions
from devutils.constants.paths import Directories as Directories
from devutils.constants.paths import Files as Files
from devutils.utils.file_checking import (
    FileResult as FileResult,
)
from devutils.utils.file_checking import (
    FileStatus as FileStatus,
)
from devutils.utils.file_checking import (
    LanguageConfig as LanguageConfig,
)
from devutils.utils.file_checking import (
    Statistics as Statistics,
)
from devutils.utils.file_checking import (
    print_status as print_status,
)

format: typer.Typer

@dataclass
class FormatLanguageConfig(LanguageConfig):
    check_args: list[str]
    fix_args: list[str]
    formatter_tool: str = ...
    package_level: bool = ...

def get_language_configs() -> list[FormatLanguageConfig]: ...
def check_tool_available(tool_name: str) -> bool: ...
def run_package_format_check(
    files: list[pathlib.Path], config: FormatLanguageConfig
) -> dict[pathlib.Path, FileResult]: ...
def check_file_formatting(
    file_path: pathlib.Path, config: FormatLanguageConfig, package_results: dict[pathlib.Path, FileResult] | None = None
) -> FileResult: ...
def fix_file_formatting(file_path: pathlib.Path, config: FormatLanguageConfig) -> bool: ...
def check_files(files: list[pathlib.Path], config: FormatLanguageConfig, stats: Statistics) -> None: ...
def fix_files(files: list[pathlib.Path], config: FormatLanguageConfig, stats: Statistics) -> None: ...
def check() -> None: ...
def fix() -> None: ...
