# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import pathlib
import subprocess
import sys
from dataclasses import dataclass

import typer

from devutils.constants import Extensions
from devutils.constants.paths import Directories, Files
from devutils.utils.file_checking import (
    FileResult,
    FileStatus,
    LanguageConfig,
    Statistics,
    print_status,
)

format: typer.Typer = typer.Typer()


@dataclass
class FormatLanguageConfig(LanguageConfig):
    check_args: list[str]
    fix_args: list[str]
    formatter_tool: str = ""
    package_level: bool = False


def get_language_configs() -> list[FormatLanguageConfig]:
    return [
        FormatLanguageConfig(
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
        FormatLanguageConfig(
            name="Python",
            extensions=Extensions.python_source,
            search_dirs=[Directories.devutils_source],
            specific_files=[],
            formatter_tool="ruff",
            check_args=[
                "--config",
                str(Files.devutils_pyproject_toml),
                "format",
                "--check",
                str(Directories.devutils_source / "devutils"),
            ],
            fix_args=[
                "--config",
                str(Files.devutils_pyproject_toml),
                "format",
                str(Directories.devutils_source / "devutils"),
            ],
            package_level=True,
        ),
    ]


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


def run_package_format_check(files: list[pathlib.Path], config: FormatLanguageConfig) -> dict[pathlib.Path, FileResult]:
    try:
        if config.formatter_tool == "ruff":
            cmd = ["uv", "run", config.formatter_tool] + config.check_args
        else:
            cmd = [config.formatter_tool] + config.check_args

        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            check=False,
        )

        output = result.stdout + result.stderr

        unformatted_files = set()
        for line in output.splitlines():
            if "Would reformat:" in line or "would reformat" in line.lower():
                parts = line.split(":", 1)
                if len(parts) == 2:
                    file_str = parts[1].strip()
                    try:
                        file_path_from_output = pathlib.Path(file_str)
                        if not file_path_from_output.is_absolute():
                            file_path_from_output = Directories.root / file_path_from_output
                        file_path_from_output = file_path_from_output.resolve()
                        unformatted_files.add(file_path_from_output)
                    except Exception:
                        pass

        file_results: dict[pathlib.Path, FileResult] = {}
        for file_path in files:
            resolved_path = file_path.resolve()
            if resolved_path in unformatted_files:
                file_results[file_path] = FileResult(file_path, FileStatus.ISSUE, "File needs formatting")
            else:
                file_results[file_path] = FileResult(file_path, FileStatus.OK)

        return file_results

    except Exception as e:
        return {file_path: FileResult(file_path, FileStatus.ERROR, str(e)) for file_path in files}


def check_file_formatting(
    file_path: pathlib.Path,
    config: FormatLanguageConfig,
    package_results: dict[pathlib.Path, FileResult] | None = None,
) -> FileResult:
    if config.package_level and package_results is not None:
        return package_results.get(file_path, FileResult(file_path, FileStatus.OK))

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
            error_output = result.stdout + result.stderr
            return FileResult(file_path, FileStatus.ISSUE, error_output.strip())

    except Exception as e:
        return FileResult(file_path, FileStatus.ERROR, str(e))


def fix_file_formatting(file_path: pathlib.Path, config: FormatLanguageConfig) -> bool:
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


def check_files(files: list[pathlib.Path], config: FormatLanguageConfig, stats: Statistics) -> None:
    package_results = None
    if config.package_level:
        package_results = run_package_format_check(files, config)

    for file_path in files:
        result = check_file_formatting(file_path, config, package_results)
        stats.record_result(result)

        if result.status == FileStatus.OK:
            print_status("[OK]", "green", file_path)
        elif result.status == FileStatus.ISSUE:
            print_status("[UNFORMATTED]", "red", file_path)
            if result.error:
                typer.echo(result.error)
                typer.echo()
        elif result.status == FileStatus.ERROR:
            print_status("[ERROR]", "yellow", file_path, result.error or "")
            if result.error:
                typer.echo(result.error)
                typer.echo()


def fix_files(files: list[pathlib.Path], config: FormatLanguageConfig, stats: Statistics) -> None:
    package_results = None
    if config.package_level:
        package_results = run_package_format_check(files, config)

    if config.package_level:
        files_to_fix = [
            f
            for f in files
            if package_results and package_results.get(f, FileResult(f, FileStatus.OK)).status != FileStatus.OK
        ]

        if files_to_fix:
            try:
                if config.formatter_tool == "ruff":
                    cmd = ["uv", "run", config.formatter_tool] + config.fix_args
                else:
                    cmd = [config.formatter_tool] + config.fix_args

                proc_result = subprocess.run(cmd, capture_output=True, text=True, check=False)
                package_fix_success = proc_result.returncode == 0

                for file_path in files:
                    stats.total += 1
                    if (
                        package_results
                        and package_results.get(file_path, FileResult(file_path, FileStatus.OK)).status == FileStatus.OK
                    ):
                        print_status("[SKIP]", "cyan", file_path)
                        stats.record_fix(False)
                    elif package_fix_success:
                        print_status("[FIXED]", "green", file_path)
                        stats.record_fix(True)
                    else:
                        print_status("[ERROR]", "yellow", file_path)
                        stats.errors += 1
            except Exception as e:
                for file_path in files:
                    stats.total += 1
                    print_status("[ERROR]", "yellow", file_path, str(e))
                    stats.errors += 1
        else:
            for file_path in files:
                stats.total += 1
                print_status("[SKIP]", "cyan", file_path)
                stats.record_fix(False)
    else:
        for file_path in files:
            result = check_file_formatting(file_path, config, package_results)
            stats.total += 1

            if result.status == FileStatus.ERROR:
                print_status("[ERROR]", "yellow", file_path, result.error or "")
                if result.error:
                    typer.echo(result.error)
                    typer.echo()
                stats.errors += 1
                continue

            if result.status == FileStatus.OK:
                print_status("[SKIP]", "cyan", file_path)
                stats.record_fix(False)
            else:
                fixed = fix_file_formatting(file_path, config)
                if fixed:
                    print_status("[FIXED]", "green", file_path)
                    stats.record_fix(True)
                else:
                    print_status("[ERROR]", "yellow", file_path)
                    if result.error:
                        typer.echo(result.error)
                        typer.echo()
                    stats.errors += 1


@format.command()  # type: ignore[misc]
def check() -> None:
    stats = Statistics(issue_label="[UNFORMATTED]")
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


@format.command()  # type: ignore[misc]
def fix() -> None:
    stats = Statistics(issue_label="[UNFORMATTED]")
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
