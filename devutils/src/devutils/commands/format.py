# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import pathlib
import subprocess
import sys
from dataclasses import dataclass
from enum import Enum

import typer

from devutils.constants import Directories, Extensions
from devutils.utils import file_checking

format = typer.Typer()


@dataclass
class LanguageConfig:
    name: str
    extensions: list[str]
    search_dirs: list[pathlib.Path]
    specific_files: list[pathlib.Path]
    formatter_tool: str
    check_args: list[str]
    fix_args: list[str]

    def collect_files(self) -> list[pathlib.Path]:
        return file_checking.collect_files(self.extensions, self.search_dirs, self.specific_files)


def get_language_configs() -> list[LanguageConfig]:
    return [
        LanguageConfig(
            name="C/C++",
            extensions=Extensions.c_source + Extensions.cpp_source,
            search_dirs=[
                Directories.logenium_source,
                Directories.logenium_include,
                Directories.xheader_source,
                Directories.xheader_include,
                Directories.xheader_tests,
                Directories.debug_source,
                Directories.debug_include,
            ],
            specific_files=[],
            formatter_tool="clang-format",
            check_args=["--dry-run", "-Werror"],
            fix_args=["-i"],
        ),
        LanguageConfig(
            name="Python",
            extensions=Extensions.python_source,
            search_dirs=[Directories.devutils_source],
            specific_files=[],
            formatter_tool="ruff",
            check_args=["format", "--check"],
            fix_args=["format"],
        ),
    ]


class FileStatus(Enum):
    OK = "ok"
    UNFORMATTED = "unformatted"
    ERROR = "error"
    UNKNOWN = "unknown"


FileResult = file_checking.FileResult


@dataclass
class Statistics:
    total: int = 0
    ok: int = 0
    unformatted: int = 0
    errors: int = 0
    fixed: int = 0
    skipped: int = 0

    def record_result(self, result: FileResult) -> None:
        self.total += 1
        if result.status == FileStatus.OK:
            self.ok += 1
        elif result.status == FileStatus.UNFORMATTED:
            self.unformatted += 1
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
            typer.echo(f"  {typer.style('[OK]', fg='green')}          {self.ok}")
            if self.unformatted > 0:
                typer.echo(f"  {typer.style('[UNFORMATTED]', fg='red')} {self.unformatted}")
            if self.errors > 0:
                typer.echo(f"  {typer.style('[ERROR]', fg='yellow')}       {self.errors}")
        elif mode == "fix":
            typer.echo(f"  {typer.style('[FIXED]', fg='green')}    {self.fixed}")
            typer.echo(f"  {typer.style('[SKIPPED]', fg='cyan')}  {self.skipped}")
            if self.errors > 0:
                typer.echo(f"  {typer.style('[ERROR]', fg='yellow')}    {self.errors}")

        typer.echo(typer.style("=" * 60, fg="cyan"))

    def has_failures(self) -> bool:
        return self.unformatted > 0 or self.errors > 0


def check_tool_available(tool_name: str) -> bool:
    try:
        if tool_name == "ruff":
            result = subprocess.run(
                ["uv", "run", "ruff", "--version"],
                capture_output=True,
                text=True,
                check=False,
            )
        else:
            result = subprocess.run(
                [tool_name, "--version"],
                capture_output=True,
                text=True,
                check=False,
            )
        return result.returncode == 0
    except FileNotFoundError:
        return False


def check_file_formatting(file_path: pathlib.Path, config: LanguageConfig) -> FileResult:
    try:
        if config.formatter_tool == "ruff":
            cmd = ["uv", "run", config.formatter_tool] + config.check_args + [str(file_path)]
        else:
            cmd = [config.formatter_tool] + config.check_args + [str(file_path)]

        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            check=False,
        )

        if result.returncode == 0:
            return FileResult(file_path, FileStatus.OK)
        else:
            return FileResult(file_path, FileStatus.UNFORMATTED)

    except Exception as e:
        return FileResult(file_path, FileStatus.ERROR, str(e))


def fix_file_formatting(file_path: pathlib.Path, config: LanguageConfig) -> bool:
    try:
        if config.formatter_tool == "ruff":
            cmd = ["uv", "run", config.formatter_tool] + config.fix_args + [str(file_path)]
        else:
            cmd = [config.formatter_tool] + config.fix_args + [str(file_path)]

        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            check=False,
        )

        return result.returncode == 0

    except Exception:
        return False


def check_files(files: list[pathlib.Path], config: LanguageConfig, stats: Statistics) -> None:
    for file_path in files:
        result = check_file_formatting(file_path, config)
        stats.record_result(result)

        if result.status == FileStatus.OK:
            file_checking.print_status("[OK]", "green", file_path)
        elif result.status == FileStatus.UNFORMATTED:
            file_checking.print_status("[UNFORMATTED]", "red", file_path)
        elif result.status == FileStatus.ERROR:
            file_checking.print_status("[ERROR]", "yellow", file_path, result.error or "")


def fix_files(files: list[pathlib.Path], config: LanguageConfig, stats: Statistics) -> None:
    for file_path in files:
        result = check_file_formatting(file_path, config)
        stats.total += 1

        if result.status == FileStatus.ERROR:
            file_checking.print_status("[ERROR]", "yellow", file_path, result.error or "")
            stats.errors += 1
            continue

        if result.status == FileStatus.OK:
            file_checking.print_status("[SKIP]", "cyan", file_path)
            stats.record_fix(False)
        else:
            fixed = fix_file_formatting(file_path, config)
            if fixed:
                file_checking.print_status("[FIXED]", "green", file_path)
                stats.record_fix(True)
            else:
                file_checking.print_status("[ERROR]", "yellow", file_path)
                stats.errors += 1


@format.command()
def check() -> None:
    stats = Statistics()
    configs = get_language_configs()

    for config in configs:
        if not check_tool_available(config.formatter_tool):
            typer.echo(
                typer.style(
                    f"\nError: {config.formatter_tool} is not available. Please ensure it is installed.",
                    fg="red",
                    bold=True,
                )
            )
            sys.exit(1)

    for config in configs:
        files = config.collect_files()
        if files:
            typer.echo(typer.style(f"\nChecking {config.name} files...", fg="cyan", bold=True))
            check_files(files, config, stats)

    stats.print_summary("check")

    if stats.has_failures():
        typer.echo(
            typer.style(
                "\nSome files are not formatted correctly.",
                fg="red",
                bold=True,
            )
        )
        typer.echo(typer.style("Run 'uv run devutils format fix' to fix them.", fg="yellow"))
        sys.exit(1)
    else:
        typer.echo(typer.style("\nAll files are formatted correctly!", fg="green", bold=True))
        sys.exit(0)


@format.command()
def fix() -> None:
    stats = Statistics()
    configs = get_language_configs()

    for config in configs:
        if not check_tool_available(config.formatter_tool):
            typer.echo(
                typer.style(
                    f"\nError: {config.formatter_tool} is not available. Please ensure it is installed.",
                    fg="red",
                    bold=True,
                )
            )
            sys.exit(1)

    for config in configs:
        files = config.collect_files()
        if files:
            typer.echo(typer.style(f"\nFormatting {config.name} files...", fg="cyan", bold=True))
            fix_files(files, config, stats)

    stats.print_summary("fix")

    if stats.errors > 0:
        typer.echo(typer.style("\nSome files could not be formatted due to errors.", fg="yellow", bold=True))
        sys.exit(1)
    elif stats.fixed > 0:
        typer.echo(typer.style(f"\nSuccessfully formatted {stats.fixed} file(s)!", fg="green", bold=True))
        sys.exit(0)
    else:
        typer.echo(typer.style("\nAll files are already formatted correctly!", fg="green", bold=True))
        sys.exit(0)
