# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import os
import pathlib
import subprocess
import sys
import threading
from concurrent.futures import ThreadPoolExecutor, as_completed
from dataclasses import dataclass
from typing import TypedDict

import typer
import yaml

from devutils.constants import Extensions
from devutils.constants.paths import Directories, Files
from devutils.utils.file_checking import (
    FileResult,
    FileStatus,
    LanguageConfig,
    Statistics,
    format_file_path,
    print_status,
)

lint: typer.Typer = typer.Typer()


class CacheEntry(TypedDict):
    mtime: float
    status: str
    error: str | None


class CacheData(TypedDict):
    version: str
    cache: dict[str, CacheEntry]


class LintCacheManager:
    cache_path: pathlib.Path
    enabled: bool
    cache_data: CacheData
    lock: threading.Lock
    package_level_results: dict[str, dict[pathlib.Path, FileResult]]

    def __init__(self, cache_path: pathlib.Path, enabled: bool = True):
        self.cache_path = cache_path
        self.enabled = enabled
        self.cache_data: CacheData = {"version": "1.0", "cache": {}}
        self.lock = threading.Lock()
        self.package_level_results = {}

        if self.enabled:
            self.load_cache()

    def load_cache(self) -> None:
        if not self.cache_path.exists():
            return

        try:
            with open(self.cache_path) as f:
                data: object = yaml.safe_load(f)
                if isinstance(data, dict):
                    version = data.get("version")
                    if isinstance(version, str) and version == "1.0":
                        if "cache" in data and isinstance(data["cache"], dict):
                            self.cache_data = data  # type: ignore[assignment]
        except (yaml.YAMLError, OSError):
            pass

    def save_cache(self) -> None:
        if not self.enabled:
            return

        self.cache_path.parent.mkdir(parents=True, exist_ok=True)

        temp_path = self.cache_path.with_suffix(".tmp")
        try:
            with open(temp_path, "w") as f:
                yaml.dump(self.cache_data, f, default_flow_style=False)
            temp_path.replace(self.cache_path)
        except OSError:
            if temp_path.exists():
                temp_path.unlink()

    def get_cache_key(self, language: str, tool_name: str, file_path: pathlib.Path) -> str:
        rel_path = format_file_path(file_path)
        return f"{language}:{tool_name}:{rel_path}"

    def get_cached_result(self, language: str, tool_name: str, file_path: pathlib.Path) -> FileResult | None:
        if not self.enabled:
            return None

        cache_key = self.get_cache_key(language, tool_name, file_path)

        with self.lock:
            entry = self.cache_data["cache"].get(cache_key)

        if not entry:
            return None

        try:
            current_mtime = file_path.stat().st_mtime
        except OSError:
            return None

        if entry["mtime"] != current_mtime:
            return None

        status_map = {
            "ok": FileStatus.OK,
            "warning": FileStatus.WARNING,
            "issue": FileStatus.ISSUE,
            "error": FileStatus.ERROR,
        }
        status = status_map.get(entry["status"], FileStatus.ERROR)
        return FileResult(file_path, status, entry.get("error"))

    def update_cache(self, language: str, tool_name: str, file_path: pathlib.Path, result: FileResult) -> None:
        if not self.enabled:
            return

        cache_key = self.get_cache_key(language, tool_name, file_path)

        try:
            mtime = file_path.stat().st_mtime
        except OSError:
            return

        status_map = {
            FileStatus.OK: "ok",
            FileStatus.WARNING: "warning",
            FileStatus.ISSUE: "issue",
            FileStatus.ERROR: "error",
        }
        entry: CacheEntry = {"mtime": mtime, "status": status_map[result.status], "error": result.error}

        with self.lock:
            self.cache_data["cache"][cache_key] = entry

    def get_package_result(self, cache_key: str, file_path: pathlib.Path) -> FileResult | None:
        with self.lock:
            results = self.package_level_results.get(cache_key)
            if results:
                return results.get(file_path)
        return None

    def set_package_results(self, cache_key: str, results: dict[pathlib.Path, FileResult]) -> None:
        with self.lock:
            self.package_level_results[cache_key] = results


@dataclass
class LintStep:
    tool_name: str
    check_args: list[str]
    fix_args: list[str]
    can_fix: bool = True
    package_level: bool = False


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
                Directories.logenium_tests,
                Directories.xheader_source,
                Directories.xheader_include,
                Directories.xheader_tests,
                Directories.debug_source,
                Directories.debug_include,
                Directories.debug_tests,
                Directories.corelib_source,
                Directories.corelib_include,
                Directories.corelib_tests,
                Directories.logging_source,
                Directories.logging_include,
                Directories.logging_tests,
            ],
            specific_files=[],
            lint_steps=[
                LintStep(
                    tool_name="clang-tidy",
                    check_args=["-p", str(Directories.build)],
                    fix_args=["-p", str(Directories.build), "--fix"],
                    can_fix=True,
                ),
                LintStep(
                    tool_name="clang-check",
                    check_args=["--analyze", "-p", str(Directories.build)],
                    fix_args=[],
                    can_fix=False,
                ),
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
                    check_args=[
                        "--config-file",
                        str(Files.devutils_pyproject_toml),
                        str(Directories.devutils_source / "devutils"),
                    ],
                    fix_args=[],
                    can_fix=False,
                    package_level=True,
                ),
                LintStep(
                    tool_name="ruff",
                    check_args=[
                        "--config",
                        str(Files.devutils_pyproject_toml),
                        "check",
                        str(Directories.devutils_source / "devutils"),
                    ],
                    fix_args=[
                        "--config",
                        str(Files.devutils_pyproject_toml),
                        "check",
                        "--fix",
                        str(Directories.devutils_source / "devutils"),
                    ],
                    can_fix=True,
                    package_level=True,
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


def run_package_level_lint(
    files: list[pathlib.Path],
    lint_step: LintStep,
    config_name: str,
    cache_manager: LintCacheManager,
) -> None:
    cache_key = f"{config_name}:{lint_step.tool_name}"

    if cache_manager.get_package_result(cache_key, files[0]) is not None:
        return

    try:
        if lint_step.tool_name in ["mypy", "ruff"]:
            cmd = ["uv", "run", lint_step.tool_name] + lint_step.check_args
        else:
            cmd = [lint_step.tool_name] + lint_step.check_args

        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            check=False,
        )

        output = result.stdout + result.stderr
        output_lower = output.lower()

        file_results: dict[pathlib.Path, FileResult] = {}

        for file_path in files:
            file_path_str = format_file_path(file_path)
            file_path_normalized = file_path_str.replace("\\", "/")
            output_has_file_mention = file_path_str in output or file_path_normalized in output

            if result.returncode == 0:
                file_results[file_path] = FileResult(file_path, FileStatus.OK)
            else:
                if output_has_file_mention:
                    lines = output.split("\n")
                    file_lines = [line for line in lines if file_path_str in line or file_path_normalized in line]
                    relevant_output = "\n".join(file_lines)

                    if (
                        "error:" in relevant_output.lower()
                        or "traceback" in output_lower
                        or "assertion" in output_lower
                    ):
                        file_results[file_path] = FileResult(file_path, FileStatus.ERROR, relevant_output)
                    elif "warning:" in relevant_output.lower() or "note:" in relevant_output.lower():
                        file_results[file_path] = FileResult(file_path, FileStatus.WARNING, relevant_output)
                    else:
                        file_results[file_path] = FileResult(file_path, FileStatus.ISSUE, relevant_output)
                else:
                    file_results[file_path] = FileResult(file_path, FileStatus.OK)

        cache_manager.set_package_results(cache_key, file_results)

    except Exception:
        for file_path in files:
            cache_manager.set_package_results(
                cache_key,
                {file_path: FileResult(file_path, FileStatus.ERROR, "Failed to run package-level linting")},
            )


def check_file_lint(
    file_path: pathlib.Path,
    lint_step: LintStep,
    config_name: str,
    cache_manager: LintCacheManager,
) -> FileResult:
    if lint_step.package_level:
        cache_key = f"{config_name}:{lint_step.tool_name}"
        result = cache_manager.get_package_result(cache_key, file_path)
        if result:
            return result
        else:
            return FileResult(file_path, FileStatus.ERROR, "Package-level linting not run")

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

        output = result.stdout + result.stderr
        output_lower = output.lower()

        if result.returncode == 0:
            if "warning:" in output_lower or "note:" in output_lower:
                return FileResult(file_path, FileStatus.WARNING, output.strip())
            else:
                return FileResult(file_path, FileStatus.OK)
        else:
            if "error:" in output_lower or "traceback" in output_lower or "assertion" in output_lower:
                return FileResult(file_path, FileStatus.ERROR, output.strip())
            elif "warning:" in output_lower or "note:" in output_lower:
                return FileResult(file_path, FileStatus.WARNING, output.strip())
            else:
                return FileResult(file_path, FileStatus.ISSUE, output.strip())

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


def check_single_file(
    file_path: pathlib.Path,
    config: LintLanguageConfig,
    cache_manager: LintCacheManager,
    output_lock: threading.Lock,
) -> tuple[FileStatus, list[str]]:
    file_has_warnings = False
    file_has_issues = False
    file_has_errors = False
    error_messages = []
    cached_count = 0

    for lint_step in config.lint_steps:
        cached_result = cache_manager.get_cached_result(config.name, lint_step.tool_name, file_path)

        if cached_result:
            result = cached_result
            cached_count += 1
        else:
            result = check_file_lint(file_path, lint_step, config.name, cache_manager)

            cache_manager.update_cache(config.name, lint_step.tool_name, file_path, result)

        if result.status == FileStatus.WARNING:
            file_has_warnings = True
            if result.error:
                error_messages.append(f"[{lint_step.tool_name}]\n{result.error}")
        elif result.status == FileStatus.ISSUE:
            file_has_issues = True
            if result.error:
                error_messages.append(f"[{lint_step.tool_name}]\n{result.error}")
        elif result.status == FileStatus.ERROR:
            file_has_errors = True
            if result.error:
                error_messages.append(f"[{lint_step.tool_name}]\n{result.error}")

    if file_has_errors:
        final_status = FileStatus.ERROR
    elif file_has_issues:
        final_status = FileStatus.ISSUE
    elif file_has_warnings:
        final_status = FileStatus.WARNING
    else:
        final_status = FileStatus.OK

    is_cached = cached_count == len(config.lint_steps)

    with output_lock:
        if final_status == FileStatus.ERROR:
            tag = "[CACHED:ERROR]" if is_cached else "[ERROR]"
            print_status(tag, "red", file_path)
            if error_messages:
                typer.echo("\n".join(error_messages))
                typer.echo()
        elif final_status == FileStatus.ISSUE:
            tag = "[CACHED:ISSUE]" if is_cached else "[HAS_ISSUES]"
            print_status(tag, "red", file_path)
            if error_messages:
                typer.echo("\n".join(error_messages))
                typer.echo()
        elif final_status == FileStatus.WARNING:
            tag = "[CACHED:WARNING]" if is_cached else "[WARNING]"
            print_status(tag, "yellow", file_path)
            if error_messages:
                typer.echo("\n".join(error_messages))
                typer.echo()
        else:
            tag = "[CACHED:OK]" if is_cached else "[OK]"
            print_status(tag, "green", file_path)

    return final_status, error_messages


def check_files_parallel(
    files: list[pathlib.Path],
    config: LintLanguageConfig,
    stats: Statistics,
    cache_manager: LintCacheManager,
) -> None:
    for lint_step in config.lint_steps:
        if lint_step.package_level:
            run_package_level_lint(files, lint_step, config.name, cache_manager)

    output_lock = threading.Lock()

    with ThreadPoolExecutor(max_workers=os.cpu_count()) as executor:
        future_to_file = {
            executor.submit(check_single_file, file_path, config, cache_manager, output_lock): file_path
            for file_path in files
        }

        for future in as_completed(future_to_file):
            file_path = future_to_file[future]
            try:
                final_status, error_messages = future.result()

                result = FileResult(file_path, final_status, "\n".join(error_messages) if error_messages else None)
                stats.record_result_threadsafe(result)

            except Exception as e:
                with output_lock:
                    print_status("[ERROR]", "yellow", file_path, str(e))
                stats.increment_total_threadsafe()
                stats.increment_errors_threadsafe()


def fix_single_file(
    file_path: pathlib.Path,
    config: LintLanguageConfig,
    cache_manager: LintCacheManager,
    output_lock: threading.Lock,
) -> tuple[str, list[str]]:
    file_had_issues = False
    all_fixed = True
    file_has_errors = False
    error_messages = []
    cached_count = 0

    for lint_step in config.lint_steps:
        cached_result = cache_manager.get_cached_result(config.name, lint_step.tool_name, file_path)

        if cached_result:
            result = cached_result
            cached_count += 1
        else:
            result = check_file_lint(file_path, lint_step, config.name, cache_manager)
            cache_manager.update_cache(config.name, lint_step.tool_name, file_path, result)

        if result.status == FileStatus.ERROR:
            file_has_errors = True
            if result.error:
                error_messages.append(f"[{lint_step.tool_name}]\n{result.error}")
            break
        elif result.status == FileStatus.ISSUE:
            file_had_issues = True
            if lint_step.can_fix:
                fixed = fix_file_lint(file_path, lint_step)
                if fixed:
                    ok_result = FileResult(file_path, FileStatus.OK, None)
                    cache_manager.update_cache(config.name, lint_step.tool_name, file_path, ok_result)
                else:
                    all_fixed = False
                    if result.error:
                        error_messages.append(f"[{lint_step.tool_name}]\n{result.error}")
            else:
                all_fixed = False
                if result.error:
                    error_messages.append(f"[{lint_step.tool_name}]\n{result.error}")

    if file_has_errors:
        outcome = "error"
    elif not file_had_issues:
        outcome = "skip"
    elif all_fixed:
        outcome = "fixed"
    else:
        outcome = "partial"

    is_cached = cached_count == len(config.lint_steps) and outcome == "skip"

    with output_lock:
        if outcome == "error":
            print_status("[ERROR]", "yellow", file_path)
            if error_messages:
                typer.echo("\n".join(error_messages))
                typer.echo()
        elif outcome == "skip":
            tag = "[CACHED:SKIP]" if is_cached else "[SKIP]"
            print_status(tag, "cyan", file_path)
        elif outcome == "fixed":
            print_status("[FIXED]", "green", file_path)
        else:
            print_status("[PARTIAL]", "yellow", file_path)
            if error_messages:
                typer.echo("\n".join(error_messages))
                typer.echo()

    return outcome, error_messages


def fix_files_parallel(
    files: list[pathlib.Path],
    config: LintLanguageConfig,
    stats: Statistics,
    cache_manager: LintCacheManager,
) -> None:
    output_lock = threading.Lock()

    with ThreadPoolExecutor(max_workers=os.cpu_count()) as executor:
        future_to_file = {
            executor.submit(fix_single_file, file_path, config, cache_manager, output_lock): file_path
            for file_path in files
        }

        for future in as_completed(future_to_file):
            file_path = future_to_file[future]
            try:
                outcome, error_messages = future.result()

                stats.increment_total_threadsafe()

                if outcome == "error":
                    stats.increment_errors_threadsafe()
                elif outcome == "skip":
                    stats.record_fix_threadsafe(False)
                elif outcome == "fixed":
                    stats.record_fix_threadsafe(True)
                else:
                    stats.record_fix_threadsafe(False)

            except Exception as e:
                with output_lock:
                    print_status("[ERROR]", "yellow", file_path, str(e))
                stats.increment_total_threadsafe()
                stats.increment_errors_threadsafe()


@lint.command()  # type: ignore[misc]
def check(no_cache: bool = typer.Option(False, "--no-cache", help="Disable caching and re-lint all files")) -> None:
    stats = Statistics(issue_label="[HAS_ISSUES]")
    configs = get_language_configs()

    cache_manager = LintCacheManager(Files.devutils_lint_cache_file, enabled=not no_cache)

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
            check_files_parallel(files, config, stats, cache_manager)

    cache_manager.save_cache()

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


@lint.command()  # type: ignore[misc]
def fix(no_cache: bool = typer.Option(False, "--no-cache", help="Disable caching and re-lint all files")) -> None:
    stats = Statistics(issue_label="[HAS_ISSUES]")
    configs = get_language_configs()

    cache_manager = LintCacheManager(Files.devutils_lint_cache_file, enabled=not no_cache)

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
            fix_files_parallel(files, config, stats, cache_manager)

    cache_manager.save_cache()

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
