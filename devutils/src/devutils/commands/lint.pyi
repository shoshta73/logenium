# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import pathlib
import threading
from dataclasses import dataclass
from typing import Any

from _typeshed import Incomplete

from devutils.constants import Directories as Directories
from devutils.constants import Extensions as Extensions
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
    format_file_path as format_file_path,
)
from devutils.utils.file_checking import (
    print_status as print_status,
)

lint: Incomplete

class LintCacheManager:
    cache_path: Incomplete
    enabled: Incomplete
    cache_data: dict[str, Any]
    lock: Incomplete
    def __init__(self, cache_path: pathlib.Path, enabled: bool = True) -> None: ...
    def load_cache(self) -> None: ...
    def save_cache(self) -> None: ...
    def get_cache_key(self, language: str, tool_name: str, file_path: pathlib.Path) -> str: ...
    def get_cached_result(self, language: str, tool_name: str, file_path: pathlib.Path) -> FileResult | None: ...
    def update_cache(self, language: str, tool_name: str, file_path: pathlib.Path, result: FileResult) -> None: ...

@dataclass
class LintStep:
    tool_name: str
    check_args: list[str]
    fix_args: list[str]
    can_fix: bool = ...

@dataclass
class LintLanguageConfig(LanguageConfig):
    lint_steps: list[LintStep]

def get_language_configs() -> list[LintLanguageConfig]: ...
def check_tool_available(tool_name: str) -> bool: ...
def check_file_lint(file_path: pathlib.Path, lint_step: LintStep) -> FileResult: ...
def fix_file_lint(file_path: pathlib.Path, lint_step: LintStep) -> bool: ...
def check_single_file(
    file_path: pathlib.Path, config: LintLanguageConfig, cache_manager: LintCacheManager, output_lock: threading.Lock
) -> tuple[FileStatus, list[str]]: ...
def check_files_parallel(
    files: list[pathlib.Path], config: LintLanguageConfig, stats: Statistics, cache_manager: LintCacheManager
) -> None: ...
def fix_single_file(
    file_path: pathlib.Path, config: LintLanguageConfig, cache_manager: LintCacheManager, output_lock: threading.Lock
) -> tuple[str, list[str]]: ...
def fix_files_parallel(
    files: list[pathlib.Path], config: LintLanguageConfig, stats: Statistics, cache_manager: LintCacheManager
) -> None: ...
def check(no_cache: bool = ...) -> None: ...
def fix(no_cache: bool = ...) -> None: ...
