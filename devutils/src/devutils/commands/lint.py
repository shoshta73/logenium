# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import pathlib
import subprocess
import sys
from dataclasses import dataclass

import typer

from devutils.constants import Directories, Extensions
from devutils.utils.file_checking import (
    FileResult,
    FileStatus,
    LanguageConfig,
    Statistics,
    print_status,
)

lint = typer.Typer()


@dataclass
class LintStep:
    tool_name: str
    check_args: list[str]
    fix_args: list[str]
    can_fix: bool = True


@dataclass
class LintLanguageConfig(LanguageConfig):
    lint_steps: list[LintStep]


def get_language_configs() -> list[LintLanguageConfig]:
    return [
        LintLanguageConfig(
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
                    check_args=["-p", str(Directories.build)],
                    fix_args=["-p", str(Directories.build), "--fix"],
                    can_fix=True,
                )
            ],
        ),
        LintLanguageConfig(
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
            error_output = result.stdout + result.stderr
            return FileResult(file_path, FileStatus.ISSUE, error_output.strip())

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


def check_files(files: list[pathlib.Path], config: LintLanguageConfig, stats: Statistics) -> None:
    for file_path in files:
        file_has_issues = False
        file_has_errors = False
        error_messages = []

        for lint_step in config.lint_steps:
            result = check_file_lint(file_path, lint_step)

            if result.status == FileStatus.ISSUE:
                file_has_issues = True
                if result.error:
                    error_messages.append(f"[{lint_step.tool_name}]\n{result.error}")
            elif result.status == FileStatus.ERROR:
                file_has_errors = True
                if result.error:
                    error_messages.append(f"[{lint_step.tool_name}]\n{result.error}")

        if file_has_errors:
            stats.total += 1
            stats.errors += 1
            print_status("[ERROR]", "yellow", file_path)
            if error_messages:
                typer.echo("\n".join(error_messages))
                typer.echo()
        elif file_has_issues:
            stats.total += 1
            stats.issues += 1
            print_status("[HAS_ISSUES]", "red", file_path)
            if error_messages:
                typer.echo("\n".join(error_messages))
                typer.echo()
        else:
            stats.total += 1
            stats.ok += 1
            print_status("[OK]", "green", file_path)


def fix_files(files: list[pathlib.Path], config: LintLanguageConfig, stats: Statistics) -> None:
    for file_path in files:
        file_had_issues = False
        all_fixed = True
        file_has_errors = False
        error_messages = []

        for lint_step in config.lint_steps:
            result = check_file_lint(file_path, lint_step)

            if result.status == FileStatus.ERROR:
                file_has_errors = True
                if result.error:
                    error_messages.append(f"[{lint_step.tool_name}]\n{result.error}")
                break
            elif result.status == FileStatus.ISSUE:
                file_had_issues = True
                if lint_step.can_fix:
                    fixed = fix_file_lint(file_path, lint_step)
                    if not fixed:
                        all_fixed = False
                        if result.error:
                            error_messages.append(f"[{lint_step.tool_name}]\n{result.error}")
                else:
                    all_fixed = False
                    if result.error:
                        error_messages.append(f"[{lint_step.tool_name}]\n{result.error}")

        stats.total += 1

        if file_has_errors:
            print_status("[ERROR]", "yellow", file_path)
            stats.errors += 1
            if error_messages:
                typer.echo("\n".join(error_messages))
                typer.echo()
        elif not file_had_issues:
            print_status("[SKIP]", "cyan", file_path)
            stats.record_fix(False)
        elif all_fixed:
            print_status("[FIXED]", "green", file_path)
            stats.record_fix(True)
        else:
            print_status("[PARTIAL]", "yellow", file_path)
            stats.record_fix(False)
            if error_messages:
                typer.echo("\n".join(error_messages))
                typer.echo()


@lint.command()
def check() -> None:
    stats = Statistics(issue_label="[HAS_ISSUES]")
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
    stats = Statistics(issue_label="[HAS_ISSUES]")
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
