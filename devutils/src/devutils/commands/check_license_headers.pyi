import pathlib
from _typeshed import Incomplete
from dataclasses import dataclass
from devutils.constants import Directories as Directories, Extensions as Extensions, LicenseHeaders as LicenseHeaders
from devutils.utils.file_checking import (
    FileResult as FileResult,
    FileStatus as FileStatus,
    LanguageConfig as LanguageConfig,
    Statistics as Statistics,
    print_status as print_status,
)
from enum import Enum

check_license_headers: Incomplete

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
