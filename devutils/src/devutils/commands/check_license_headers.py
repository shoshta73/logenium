# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import pathlib
import sys
from dataclasses import dataclass
from enum import Enum

import typer

from devutils.constants import Directories, Extensions, LicenseHeaders
from devutils.utils.file_checking import (
    FileResult,
    FileStatus,
    LanguageConfig,
    Statistics,
    print_status,
)

check_license_headers = typer.Typer()


@dataclass
class LicenseLanguageConfig(LanguageConfig):
    license_header: list[str]


def get_language_configs() -> list[LicenseLanguageConfig]:
    return [
        LicenseLanguageConfig(
            name="C/C++",
            extensions=Extensions.c_source + Extensions.cpp_source,
            search_dirs=[Directories.logenium_source],
            specific_files=[],
            license_header=LicenseHeaders.cpp,
        ),
        LicenseLanguageConfig(
            name="CMake",
            extensions=Extensions.cmake_source,
            search_dirs=[Directories.logenium_cmake, Directories.xheader_cmake, Directories.debug_cmake],
            specific_files=[
                Directories.root / "CMakeLists.txt",
                Directories.libs / "CMakeLists.txt",
                Directories.xheader_root / "CMakeLists.txt",
                Directories.xheader_tests / "CMakeLists.txt",
                Directories.debug_root / "CMakeLists.txt",
                Directories.debug_tests / "CMakeLists.txt",
            ],
            license_header=LicenseHeaders.cmake,
        ),
        LicenseLanguageConfig(
            name="Python",
            extensions=Extensions.python_source,
            search_dirs=[Directories.devutils_source],
            specific_files=[],
            license_header=LicenseHeaders.python,
        ),
        LicenseLanguageConfig(
            name="Batch",
            extensions=Extensions.bat_source,
            search_dirs=[],
            specific_files=[Directories.root / "devutils.bat"],
            license_header=LicenseHeaders.bat,
        ),
        LicenseLanguageConfig(
            name="PowerShell",
            extensions=Extensions.powershell_source,
            search_dirs=[],
            specific_files=[Directories.root / "devutils.ps1"],
            license_header=LicenseHeaders.powershell,
        ),
        LicenseLanguageConfig(
            name="Bash",
            extensions=Extensions.bash_source,
            search_dirs=[],
            specific_files=[Directories.root / "devutils.sh"],
            license_header=LicenseHeaders.bash,
        ),
    ]


class IssueType(Enum):
    MISSING = "missing"
    INCORRECT = "incorrect"


class LicenseHeaderStatistics(Statistics):
    def __init__(self) -> None:
        super().__init__()
        self.missing: int = 0
        self.incorrect: int = 0

    def record_result(self, result: FileResult, issue_type: IssueType | None = None) -> None:
        self.total += 1
        if result.status == FileStatus.OK:
            self.ok += 1
        elif result.status == FileStatus.ISSUE:
            self.issues += 1
            if issue_type == IssueType.MISSING:
                self.missing += 1
            elif issue_type == IssueType.INCORRECT:
                self.incorrect += 1
        elif result.status == FileStatus.ERROR:
            self.errors += 1

    def print_summary(self, mode: str) -> None:
        typer.echo("")
        typer.echo(typer.style("=" * 60, fg="cyan"))
        typer.echo(typer.style(f"Summary ({mode} mode)", fg="cyan", bold=True))
        typer.echo(typer.style("=" * 60, fg="cyan"))
        typer.echo(f"Total files checked: {self.total}")

        if mode == "check":
            typer.echo(f"  {typer.style('[OK]', fg='green')}        {self.ok}")
            if self.missing > 0:
                typer.echo(f"  {typer.style('[MISSING]', fg='red')}   {self.missing}")
            if self.incorrect > 0:
                typer.echo(f"  {typer.style('[INCORRECT]', fg='red')} {self.incorrect}")
            if self.errors > 0:
                typer.echo(f"  {typer.style('[ERROR]', fg='yellow')}     {self.errors}")
        elif mode == "fix":
            typer.echo(f"  {typer.style('[FIXED]', fg='green')}    {self.fixed}")
            typer.echo(f"  {typer.style('[SKIPPED]', fg='cyan')}   {self.skipped}")
            if self.errors > 0:
                typer.echo(f"  {typer.style('[ERROR]', fg='yellow')}     {self.errors}")

        typer.echo(typer.style("=" * 60, fg="cyan"))

    def has_failures(self) -> bool:
        return self.missing > 0 or self.incorrect > 0 or self.errors > 0


@dataclass
class HeaderCheckResult:
    result: FileResult
    issue_type: IssueType | None = None


def has_correct_license_header(file_path: pathlib.Path, expected_header: list[str]) -> HeaderCheckResult:
    try:
        with open(file_path, encoding="utf-8") as f:
            lines = f.readlines()

        if len(lines) < len(expected_header):
            return HeaderCheckResult(FileResult(file_path, FileStatus.ISSUE), IssueType.MISSING)

        for i, expected_line in enumerate(expected_header):
            if lines[i] != expected_line:
                return HeaderCheckResult(FileResult(file_path, FileStatus.ISSUE), IssueType.INCORRECT)

        return HeaderCheckResult(FileResult(file_path, FileStatus.OK))

    except PermissionError:
        return HeaderCheckResult(FileResult(file_path, FileStatus.ERROR, "Permission denied"))
    except UnicodeDecodeError:
        return HeaderCheckResult(FileResult(file_path, FileStatus.ERROR, "Unicode decode error"))
    except Exception as e:
        return HeaderCheckResult(FileResult(file_path, FileStatus.ERROR, str(e)))


def fix_header(file_path: pathlib.Path, expected_header: list[str]) -> bool:
    try:
        with open(file_path, encoding="utf-8") as f:
            lines = f.readlines()

        if len(lines) >= len(expected_header):
            header_correct = all(lines[i] == expected_header[i] for i in range(len(expected_header)))
            if header_correct:
                return False

        with open(file_path, "w", encoding="utf-8") as f:
            f.writelines(expected_header)
            f.writelines(lines)

        return True

    except (PermissionError, UnicodeDecodeError, Exception) as e:
        typer.echo(f"  {typer.style('[ERROR]', fg='yellow')} Failed to fix {file_path}: {e}")
        return False


def check_files(files: list[pathlib.Path], header_lines: list[str], stats: LicenseHeaderStatistics) -> None:
    for file_path in files:
        check_result = has_correct_license_header(file_path, header_lines)
        stats.record_result(check_result.result, check_result.issue_type)

        if check_result.result.status == FileStatus.OK:
            print_status("[OK]", "green", file_path)
        elif check_result.result.status == FileStatus.ISSUE:
            if check_result.issue_type == IssueType.MISSING:
                print_status("[MISSING]", "red", file_path)
            elif check_result.issue_type == IssueType.INCORRECT:
                print_status("[INCORRECT]", "red", file_path)
        elif check_result.result.status == FileStatus.ERROR:
            print_status("[ERROR]", "yellow", file_path, check_result.result.error or "")


def fix_files(files: list[pathlib.Path], header_lines: list[str], stats: LicenseHeaderStatistics) -> None:
    for file_path in files:
        check_result = has_correct_license_header(file_path, header_lines)
        stats.total += 1

        if check_result.result.status == FileStatus.ERROR:
            print_status("[ERROR]", "yellow", file_path, check_result.result.error or "")
            stats.errors += 1
            continue

        if check_result.result.status == FileStatus.OK:
            print_status("[SKIP]", "cyan", file_path)
            stats.record_fix(False)
        else:
            fixed = fix_header(file_path, header_lines)
            if fixed:
                print_status("[FIXED]", "green", file_path)
                stats.record_fix(True)
            else:
                print_status("[SKIP]", "cyan", file_path)
                stats.record_fix(False)


@check_license_headers.command()
def show_headers() -> None:
    for config in get_language_configs():
        typer.echo(f"{config.name} License Header:")
        typer.echo("".join(config.license_header[:2]) + "\n")


@check_license_headers.command()
def check() -> None:
    stats = LicenseHeaderStatistics()

    for config in get_language_configs():
        files = config.collect_files()
        if files:
            check_files(files, config.license_header, stats)

    stats.print_summary("check")

    if stats.has_failures():
        typer.echo(
            typer.style(
                "\nSome files are missing or have incorrect license headers.",
                fg="red",
                bold=True,
            )
        )
        typer.echo(typer.style("Run 'uv run devutils cls fix' to fix them.", fg="yellow"))
        sys.exit(1)
    else:
        typer.echo(typer.style("\nAll files have correct license headers!", fg="green", bold=True))
        sys.exit(0)


@check_license_headers.command()
def fix() -> None:
    stats = LicenseHeaderStatistics()

    for config in get_language_configs():
        files = config.collect_files()
        if files:
            fix_files(files, config.license_header, stats)

    stats.print_summary("fix")

    if stats.errors > 0:
        typer.echo(typer.style("\nSome files could not be fixed due to errors.", fg="yellow", bold=True))
        sys.exit(1)
    elif stats.fixed > 0:
        typer.echo(typer.style(f"\nSuccessfully fixed {stats.fixed} file(s)!", fg="green", bold=True))
        sys.exit(0)
    else:
        typer.echo(typer.style("\nAll files already have correct license headers!", fg="green", bold=True))
        sys.exit(0)
