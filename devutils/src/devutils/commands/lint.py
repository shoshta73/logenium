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

lint = typer.Typer()


@dataclass
class LintStep:
    tool_name: str
    check_args: list[str]
    fix_args: list[str]
    can_fix: bool = True


@dataclass
class LanguageConfig:
    name: str
    extensions: list[str]
    search_dirs: list[pathlib.Path]
    specific_files: list[pathlib.Path]
    lint_steps: list[LintStep]

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
            lint_steps=[
                LintStep(
                    tool_name="clang-tidy",
                    check_args=[],
                    fix_args=["--fix"],
                    can_fix=True,
                )
            ],
        ),
        LanguageConfig(
            name="Python",
            extensions=Extensions.python_source,
            search_dirs=[Directories.devutils_source],
            specific_files=[],
            lint_steps=[
                LintStep(
                    tool_name="mypy",
                    check_args=[],
                    fix_args=[],
                    can_fix=False,
                ),
                LintStep(
                    tool_name="ruff",
                    check_args=["check"],
                    fix_args=["check", "--fix"],
                    can_fix=True,
                ),
            ],
        ),
    ]


class FileStatus(Enum):
    OK = "ok"
    HAS_ISSUES = "has_issues"
    ERROR = "error"
    UNKNOWN = "unknown"


FileResult = file_checking.FileResult


@dataclass
class Statistics:
    total: int = 0
    ok: int = 0
    has_issues: int = 0
    errors: int = 0
    fixed: int = 0
    skipped: int = 0

    def record_result(self, result: FileResult) -> None:
        self.total += 1
        if result.status == FileStatus.OK:
            self.ok += 1
        elif result.status == FileStatus.HAS_ISSUES:
            self.has_issues += 1
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
            if self.has_issues > 0:
                typer.echo(f"  {typer.style('[HAS_ISSUES]', fg='red')} {self.has_issues}")
            if self.errors > 0:
                typer.echo(f"  {typer.style('[ERROR]', fg='yellow')}      {self.errors}")
        elif mode == "fix":
            typer.echo(f"  {typer.style('[FIXED]', fg='green')}    {self.fixed}")
            typer.echo(f"  {typer.style('[SKIPPED]', fg='cyan')}  {self.skipped}")
            if self.errors > 0:
                typer.echo(f"  {typer.style('[ERROR]', fg='yellow')}    {self.errors}")

        typer.echo(typer.style("=" * 60, fg="cyan"))

    def has_failures(self) -> bool:
        return self.has_issues > 0 or self.errors > 0


def check_tool_available(tool_name: str) -> bool:
    try:
        if tool_name in ["mypy", "ruff"]:
            result = subprocess.run(
                ["uv", "run", tool_name, "--version"],
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


def check_file_lint(file_path: pathlib.Path, lint_step: LintStep) -> FileResult:
    try:
        if lint_step.tool_name in ["mypy", "ruff"]:
            cmd = ["uv", "run", lint_step.tool_name] + lint_step.check_args + [str(file_path)]
        else:
            cmd = [lint_step.tool_name] + lint_step.check_args + [str(file_path)]

        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            check=False,
        )

        if result.returncode == 0:
            return FileResult(file_path, FileStatus.OK)
        else:
            return FileResult(file_path, FileStatus.HAS_ISSUES)

    except Exception as e:
        return FileResult(file_path, FileStatus.ERROR, str(e))


def fix_file_lint(file_path: pathlib.Path, lint_step: LintStep) -> bool:
    try:
        if not lint_step.can_fix:
            return False

        if lint_step.tool_name in ["mypy", "ruff"]:
            cmd = ["uv", "run", lint_step.tool_name] + lint_step.fix_args + [str(file_path)]
        else:
            cmd = [lint_step.tool_name] + lint_step.fix_args + [str(file_path)]

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
        file_has_issues = False
        file_has_errors = False

        for lint_step in config.lint_steps:
            result = check_file_lint(file_path, lint_step)

            if result.status == FileStatus.HAS_ISSUES:
                file_has_issues = True
            elif result.status == FileStatus.ERROR:
                file_has_errors = True

        if file_has_errors:
            stats.total += 1
            stats.errors += 1
            file_checking.print_status("[ERROR]", "yellow", file_path)
        elif file_has_issues:
            stats.total += 1
            stats.has_issues += 1
            file_checking.print_status("[HAS_ISSUES]", "red", file_path)
        else:
            stats.total += 1
            stats.ok += 1
            file_checking.print_status("[OK]", "green", file_path)


def fix_files(files: list[pathlib.Path], config: LanguageConfig, stats: Statistics) -> None:
    for file_path in files:
        file_had_issues = False
        all_fixed = True
        file_has_errors = False

        for lint_step in config.lint_steps:
            result = check_file_lint(file_path, lint_step)

            if result.status == FileStatus.ERROR:
                file_has_errors = True
                break
            elif result.status == FileStatus.HAS_ISSUES:
                file_had_issues = True
                if lint_step.can_fix:
                    fixed = fix_file_lint(file_path, lint_step)
                    if not fixed:
                        all_fixed = False
                else:
                    all_fixed = False

        stats.total += 1

        if file_has_errors:
            file_checking.print_status("[ERROR]", "yellow", file_path)
            stats.errors += 1
        elif not file_had_issues:
            file_checking.print_status("[SKIP]", "cyan", file_path)
            stats.record_fix(False)
        elif all_fixed:
            file_checking.print_status("[FIXED]", "green", file_path)
            stats.record_fix(True)
        else:
            file_checking.print_status("[PARTIAL]", "yellow", file_path)
            stats.record_fix(False)


@lint.command()
def check() -> None:
    stats = Statistics()
    configs = get_language_configs()

    for config in configs:
        for lint_step in config.lint_steps:
            if not check_tool_available(lint_step.tool_name):
                typer.echo(
                    typer.style(
                        f"\nError: {lint_step.tool_name} is not available. Please ensure it is installed.",
                        fg="red",
                        bold=True,
                    )
                )
                sys.exit(1)

    for config in configs:
        files = config.collect_files()
        if files:
            typer.echo(typer.style(f"\nLinting {config.name} files...", fg="cyan", bold=True))
            check_files(files, config, stats)

    stats.print_summary("check")

    if stats.has_failures():
        typer.echo(
            typer.style(
                "\nSome files have linting issues.",
                fg="red",
                bold=True,
            )
        )
        typer.echo(typer.style("Run 'uv run devutils lint fix' to fix them.", fg="yellow"))
        sys.exit(1)
    else:
        typer.echo(typer.style("\nAll files passed linting checks!", fg="green", bold=True))
        sys.exit(0)


@lint.command()
def fix() -> None:
    stats = Statistics()
    configs = get_language_configs()

    for config in configs:
        for lint_step in config.lint_steps:
            if not check_tool_available(lint_step.tool_name):
                typer.echo(
                    typer.style(
                        f"\nError: {lint_step.tool_name} is not available. Please ensure it is installed.",
                        fg="red",
                        bold=True,
                    )
                )
                sys.exit(1)

    for config in configs:
        files = config.collect_files()
        if files:
            typer.echo(typer.style(f"\nLinting {config.name} files...", fg="cyan", bold=True))
            fix_files(files, config, stats)

    stats.print_summary("fix")

    if stats.errors > 0:
        typer.echo(typer.style("\nSome files could not be fixed due to errors.", fg="yellow", bold=True))
        sys.exit(1)
    elif stats.fixed > 0:
        typer.echo(typer.style(f"\nSuccessfully fixed {stats.fixed} file(s)!", fg="green", bold=True))
        sys.exit(0)
    else:
        typer.echo(typer.style("\nAll files already pass linting checks!", fg="green", bold=True))
        sys.exit(0)
