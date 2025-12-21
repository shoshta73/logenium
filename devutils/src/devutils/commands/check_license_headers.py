# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import pathlib
import sys
from dataclasses import dataclass
from enum import Enum

import typer

from devutils.constants import Directories, Extensions, LicenseHeaders
from devutils.utils import fs

check_license_headers = typer.Typer()


@dataclass
class LanguageConfig:
    name: str
    extensions: list[str]
    search_dirs: list[pathlib.Path]
    specific_files: list[pathlib.Path]
    license_header: list[str]

    def collect_files(self) -> list[pathlib.Path]:
        files: list[pathlib.Path] = []

        for search_dir in self.search_dirs:
            if search_dir.exists():
                files.extend(fs.find_files_by_extensions(search_dir, self.extensions))

        for specific_file in self.specific_files:
            if specific_file.exists():
                files.append(specific_file)

        return files


def get_language_configs() -> list[LanguageConfig]:
    return [
        LanguageConfig(
            name="C/C++",
            extensions=Extensions.c_source + Extensions.cpp_source,
            search_dirs=[Directories.logenium_source],
            specific_files=[],
            license_header=LicenseHeaders.cpp,
        ),
        LanguageConfig(
            name="CMake",
            extensions=Extensions.cmake_source,
            search_dirs=[Directories.logenium_cmake, Directories.xheader_cmake],
            specific_files=[
                Directories.root / "CMakeLists.txt",
                Directories.libs / "CMakeLists.txt",
                Directories.xheader_root / "CMakeLists.txt",
                Directories.xheader_tests / "CMakeLists.txt",
                Directories.debug_root / "CMakeLists.txt",
            ],
            license_header=LicenseHeaders.cmake,
        ),
        LanguageConfig(
            name="Python",
            extensions=Extensions.python_source,
            search_dirs=[Directories.devutils_source],
            specific_files=[],
            license_header=LicenseHeaders.python,
        ),
        LanguageConfig(
            name="Batch",
            extensions=Extensions.bat_source,
            search_dirs=[],
            specific_files=[Directories.root / "devutils.bat"],
            license_header=LicenseHeaders.bat,
        ),
        LanguageConfig(
            name="PowerShell",
            extensions=Extensions.powershell_source,
            search_dirs=[],
            specific_files=[Directories.root / "devutils.ps1"],
            license_header=LicenseHeaders.powershell,
        ),
    ]


class FileStatus(Enum):
    OK = "ok"
    MISSING = "missing"
    INCORRECT = "incorrect"
    ERROR = "error"
    UNKNOWN = "unknown"


@dataclass
class FileResult:
    path: pathlib.Path
    status: FileStatus
    error: str | None = None


@dataclass
class Statistics:
    total: int = 0
    ok: int = 0
    missing: int = 0
    incorrect: int = 0
    errors: int = 0
    fixed: int = 0
    skipped: int = 0

    def record_result(self, result: FileResult) -> None:
        self.total += 1
        if result.status == FileStatus.OK:
            self.ok += 1
        elif result.status == FileStatus.MISSING:
            self.missing += 1
        elif result.status == FileStatus.INCORRECT:
            self.incorrect += 1
        elif result.status == FileStatus.ERROR:
            self.errors += 1

    def record_fix(self, fixed: bool) -> None:
        if fixed:
            self.fixed += 1
        else:
            self.skipped += 1

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


def has_correct_license_header(file_path: pathlib.Path, expected_header: list[str]) -> FileResult:
    try:
        with open(file_path, encoding="utf-8") as f:
            lines = f.readlines()

        if len(lines) < len(expected_header):
            return FileResult(file_path, FileStatus.MISSING)

        for i, expected_line in enumerate(expected_header):
            if lines[i] != expected_line:
                return FileResult(file_path, FileStatus.INCORRECT)

        return FileResult(file_path, FileStatus.OK)

    except PermissionError:
        return FileResult(file_path, FileStatus.ERROR, "Permission denied")
    except UnicodeDecodeError:
        return FileResult(file_path, FileStatus.ERROR, "Unicode decode error")
    except Exception as e:
        return FileResult(file_path, FileStatus.ERROR, str(e))


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


def check_files(files: list[pathlib.Path], header_lines: list[str], stats: Statistics) -> None:
    for file_path in files:
        result = has_correct_license_header(file_path, header_lines)
        stats.record_result(result)

        if result.status == FileStatus.OK:
            typer.echo(f"{typer.style('[OK]', fg='green')}        {file_path.relative_to(Directories.root)}")
        elif result.status == FileStatus.MISSING:
            typer.echo(f"{typer.style('[MISSING]', fg='red')}   {file_path.relative_to(Directories.root)}")
        elif result.status == FileStatus.INCORRECT:
            typer.echo(f"{typer.style('[INCORRECT]', fg='red')} {file_path.relative_to(Directories.root)}")
        elif result.status == FileStatus.ERROR:
            typer.echo(
                f"{typer.style('[ERROR]', fg='yellow')}     {file_path.relative_to(Directories.root)}: {result.error}"
            )


def fix_files(files: list[pathlib.Path], header_lines: list[str], stats: Statistics) -> None:
    for file_path in files:
        result = has_correct_license_header(file_path, header_lines)
        stats.total += 1

        if result.status == FileStatus.ERROR:
            typer.echo(
                f"{typer.style('[ERROR]', fg='yellow')}   {file_path.relative_to(Directories.root)}: {result.error}"
            )
            stats.errors += 1
            continue

        if result.status == FileStatus.OK:
            typer.echo(f"{typer.style('[SKIP]', fg='cyan')}    {file_path.relative_to(Directories.root)}")
            stats.record_fix(False)
        else:
            fixed = fix_header(file_path, header_lines)
            if fixed:
                typer.echo(f"{typer.style('[FIXED]', fg='green')}   {file_path.relative_to(Directories.root)}")
                stats.record_fix(True)
            else:
                typer.echo(f"{typer.style('[SKIP]', fg='cyan')}    {file_path.relative_to(Directories.root)}")
                stats.record_fix(False)




@check_license_headers.command()
def show_headers() -> None:
    for config in get_language_configs():
        typer.echo(f"{config.name} License Header:")
        typer.echo("".join(config.license_header[:2]) + "\n")


@check_license_headers.command()
def check() -> None:
    stats = Statistics()
    stats.print_summary("check")

    for config in get_language_configs():
        files = config.collect_files()
        if files:
            check_files(files, config.license_header, stats)

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
    stats = Statistics()
    stats.print_summary("fix")

    for config in get_language_configs():
        files = config.collect_files()
        if files:
            fix_files(files, config.license_header, stats)

    if stats.errors > 0:
        typer.echo(typer.style("\nSome files could not be fixed due to errors.", fg="yellow", bold=True))
        sys.exit(1)
    elif stats.fixed > 0:
        typer.echo(typer.style(f"\nSuccessfully fixed {stats.fixed} file(s)!", fg="green", bold=True))
        sys.exit(0)
    else:
        typer.echo(typer.style("\nAll files already have correct license headers!", fg="green", bold=True))
        sys.exit(0)
