# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import pathlib
import threading
from collections.abc import Callable as Callable
from dataclasses import dataclass
from enum import Enum
from typing import TypedDict

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
    format_file_path as format_file_path,
)
from devutils.utils.file_checking import (
    print_status as print_status,
)
from devutils.utils.git import get_file_copyright_year as get_file_copyright_year

check_license_headers: typer.Typer

class CacheEntry(TypedDict):
    mtime: float
    status: str
    year: int
    error: str | None

class CacheData(TypedDict):
    version: str
    cache: dict[str, CacheEntry]

class LicenseHeaderCacheManager:
    cache_path: pathlib.Path
    enabled: bool
    cache_data: CacheData
    lock: threading.Lock
    year_cache: dict[pathlib.Path, int]
    def __init__(self, cache_path: pathlib.Path, enabled: bool = True) -> None: ...
    def load_cache(self) -> None: ...
    def save_cache(self) -> None: ...
    def get_cache_key(self, language: str, file_path: pathlib.Path) -> str: ...
    def get_cached_result(self, language: str, file_path: pathlib.Path) -> HeaderCheckResult | None: ...
    def update_cache(self, language: str, file_path: pathlib.Path, result: HeaderCheckResult) -> None: ...
    def get_cached_year(self, file_path: pathlib.Path) -> int | None: ...
    def cache_year(self, file_path: pathlib.Path, year: int) -> None: ...

@dataclass
class LicenseLanguageConfig(LanguageConfig):
    header_generator: Callable[[int], list[str]]

def get_language_configs() -> list[LicenseLanguageConfig]: ...

class IssueType(Enum):
    MISSING = "missing"
    INCORRECT = "incorrect"

class LicenseHeaderStatistics(Statistics):
    missing: int
    incorrect: int
    def __init__(self) -> None: ...
    def record_result(self, result: FileResult, issue_type: IssueType | None = None) -> None: ...
    def print_summary(self, mode: str) -> None: ...
    def has_failures(self) -> bool: ...

@dataclass
class HeaderCheckResult:
    result: FileResult
    issue_type: IssueType | None = ...
    year: int = ...

def has_correct_license_header(
    file_path: pathlib.Path,
    header_generator: Callable[[int], list[str]],
    cache_manager: LicenseHeaderCacheManager | None = None,
) -> HeaderCheckResult: ...
def fix_header(file_path: pathlib.Path, header_generator: Callable[[int], list[str]], year: int) -> bool: ...
def check_files(
    files: list[pathlib.Path], header_generator: Callable[[int], list[str]], stats: LicenseHeaderStatistics
) -> None: ...
def fix_files(
    files: list[pathlib.Path], header_generator: Callable[[int], list[str]], stats: LicenseHeaderStatistics
) -> None: ...
def check_single_file(
    file_path: pathlib.Path,
    header_generator: Callable[[int], list[str]],
    language_name: str,
    cache_manager: LicenseHeaderCacheManager,
    output_lock: threading.Lock,
) -> tuple[FileStatus, IssueType | None]: ...
def check_files_parallel(
    files: list[pathlib.Path],
    header_generator: Callable[[int], list[str]],
    language_name: str,
    stats: LicenseHeaderStatistics,
    cache_manager: LicenseHeaderCacheManager,
) -> None: ...
def fix_single_file(
    file_path: pathlib.Path,
    header_generator: Callable[[int], list[str]],
    language_name: str,
    cache_manager: LicenseHeaderCacheManager,
    output_lock: threading.Lock,
) -> str: ...
def fix_files_parallel(
    files: list[pathlib.Path],
    header_generator: Callable[[int], list[str]],
    language_name: str,
    stats: LicenseHeaderStatistics,
    cache_manager: LicenseHeaderCacheManager,
) -> None: ...
def show_headers() -> None: ...
def check(no_cache: bool = ...) -> None: ...
def fix(no_cache: bool = ...) -> None: ...
