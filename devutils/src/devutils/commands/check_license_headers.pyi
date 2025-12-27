# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import pathlib
from dataclasses import dataclass
from enum import Enum

import typer

from devutils.constants import Extensions as Extensions
from devutils.constants import LicenseHeaders as LicenseHeaders
from devutils.constants.paths import Directories as Directories
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

check_license_headers: typer.Typer

@dataclass
class LicenseLanguageConfig(LanguageConfig):
    license_header: list[str]

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

def has_correct_license_header(file_path: pathlib.Path, expected_header: list[str]) -> HeaderCheckResult: ...
def fix_header(file_path: pathlib.Path, expected_header: list[str]) -> bool: ...
def check_files(files: list[pathlib.Path], header_lines: list[str], stats: LicenseHeaderStatistics) -> None: ...
def fix_files(files: list[pathlib.Path], header_lines: list[str], stats: LicenseHeaderStatistics) -> None: ...
def show_headers() -> None: ...
def check() -> None: ...
def fix() -> None: ...
