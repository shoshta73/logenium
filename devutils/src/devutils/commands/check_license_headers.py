# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import os
import pathlib
import sys
import threading
from collections.abc import Callable
from concurrent.futures import ThreadPoolExecutor, as_completed
from dataclasses import dataclass
from enum import Enum
from typing import TypedDict

import typer
import yaml

from devutils.constants import Extensions
from devutils.constants import license_header as lh
from devutils.constants.paths import Directories, Files
from devutils.utils.file_checking import (
    FileResult,
    FileStatus,
    LanguageConfig,
    Statistics,
    format_file_path,
    print_status,
)
from devutils.utils.filesystem import find_files_by_name
from devutils.utils.git import get_file_copyright_year

check_license_headers: typer.Typer = typer.Typer()


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

    def __init__(self, cache_path: pathlib.Path, enabled: bool = True):
        self.cache_path = cache_path
        self.enabled = enabled
        self.cache_data: CacheData = {"version": "1.0", "cache": {}}
        self.lock = threading.Lock()
        self.year_cache = {}

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

    def get_cache_key(self, language: str, file_path: pathlib.Path) -> str:
        rel_path = format_file_path(file_path)
        return f"license:{language}:{rel_path}"

    def get_cached_result(self, language: str, file_path: pathlib.Path) -> HeaderCheckResult | None:
        if not self.enabled:
            return None

        cache_key = self.get_cache_key(language, file_path)

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

        cached_status = entry["status"]

        if cached_status == "ok":
            return HeaderCheckResult(FileResult(file_path, FileStatus.OK), None, entry["year"])
        elif cached_status == "missing":
            return HeaderCheckResult(FileResult(file_path, FileStatus.ISSUE), IssueType.MISSING, entry["year"])
        elif cached_status == "incorrect":
            return HeaderCheckResult(FileResult(file_path, FileStatus.ISSUE), IssueType.INCORRECT, entry["year"])
        elif cached_status == "error":
            return HeaderCheckResult(FileResult(file_path, FileStatus.ERROR, entry.get("error")), None, entry["year"])

        return None

    def update_cache(self, language: str, file_path: pathlib.Path, result: HeaderCheckResult) -> None:
        if not self.enabled:
            return

        cache_key = self.get_cache_key(language, file_path)

        try:
            mtime = file_path.stat().st_mtime
        except OSError:
            return

        if result.result.status == FileStatus.OK:
            status = "ok"
        elif result.issue_type == IssueType.MISSING:
            status = "missing"
        elif result.issue_type == IssueType.INCORRECT:
            status = "incorrect"
        else:
            status = "error"

        entry: CacheEntry = {
            "mtime": mtime,
            "status": status,
            "year": result.year,
            "error": result.result.error,
        }

        with self.lock:
            self.cache_data["cache"][cache_key] = entry

    def get_cached_year(self, file_path: pathlib.Path) -> int | None:
        with self.lock:
            return self.year_cache.get(file_path)

    def cache_year(self, file_path: pathlib.Path, year: int) -> None:
        with self.lock:
            self.year_cache[file_path] = year


@dataclass
class LicenseLanguageConfig(LanguageConfig):
    header_generator: Callable[[int], list[str]]


def get_language_configs() -> list[LicenseLanguageConfig]:
    cmake_specific_files = [
        Directories.root / "CMakeLists.txt",
        Directories.libs / "CMakeLists.txt",
        Directories.xheader_root / "CMakeLists.txt",
        Directories.debug_root / "CMakeLists.txt",
        Directories.corelib_root / "CMakeLists.txt",
    ]

    cmake_specific_files.extend(find_files_by_name(Directories.xheader_tests, "CMakeLists.txt"))
    cmake_specific_files.extend(find_files_by_name(Directories.debug_tests, "CMakeLists.txt"))
    cmake_specific_files.extend(find_files_by_name(Directories.corelib_tests, "CMakeLists.txt"))

    return [
        LicenseLanguageConfig(
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
                Directories.debug_tests,
                Directories.corelib_source,
                Directories.corelib_include,
                Directories.corelib_tests,
            ],
            specific_files=[],
            header_generator=lh.generate_cpp_header,
        ),
        LicenseLanguageConfig(
            name="CMake",
            extensions=Extensions.cmake_source,
            search_dirs=[
                Directories.logenium_cmake,
                Directories.xheader_cmake,
                Directories.debug_cmake,
                Directories.corelib_cmake,
            ],
            specific_files=cmake_specific_files,
            header_generator=lh.generate_cmake_header,
        ),
        LicenseLanguageConfig(
            name="Python",
            extensions=Extensions.python_source,
            search_dirs=[Directories.devutils_source],
            specific_files=[],
            header_generator=lh.generate_python_header,
        ),
        LicenseLanguageConfig(
            name="Batch",
            extensions=Extensions.bat_source,
            search_dirs=[],
            specific_files=[Directories.root / "devutils.bat"],
            header_generator=lh.generate_bat_header,
        ),
        LicenseLanguageConfig(
            name="PowerShell",
            extensions=Extensions.powershell_source,
            search_dirs=[],
            specific_files=[Directories.root / "devutils.ps1"],
            header_generator=lh.generate_powershell_header,
        ),
        LicenseLanguageConfig(
            name="Bash",
            extensions=Extensions.bash_source,
            search_dirs=[],
            specific_files=[Directories.root / "devutils.sh"],
            header_generator=lh.generate_bash_header,
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
    year: int = 0


def has_correct_license_header(
    file_path: pathlib.Path,
    header_generator: Callable[[int], list[str]],
    cache_manager: LicenseHeaderCacheManager | None = None,
) -> HeaderCheckResult:
    try:
        cached_year = cache_manager.get_cached_year(file_path) if cache_manager else None
        if cached_year is not None:
            year = cached_year
        else:
            year = get_file_copyright_year(file_path)
            if cache_manager:
                cache_manager.cache_year(file_path, year)

        expected_header = header_generator(year)

        with open(file_path, encoding="utf-8") as f:
            lines = f.readlines()

        header_offset = 0
        if lines and lines[0].startswith("#!"):
            header_offset = 1

        min_lines = len(expected_header) + header_offset
        if len(lines) < min_lines:
            return HeaderCheckResult(FileResult(file_path, FileStatus.ISSUE), IssueType.MISSING, year)

        for i, expected_line in enumerate(expected_header):
            if lines[i + header_offset] != expected_line:
                return HeaderCheckResult(FileResult(file_path, FileStatus.ISSUE), IssueType.INCORRECT, year)

        return HeaderCheckResult(FileResult(file_path, FileStatus.OK), None, year)

    except PermissionError:
        from datetime import datetime

        return HeaderCheckResult(
            FileResult(file_path, FileStatus.ERROR, "Permission denied"), None, datetime.now().year
        )
    except UnicodeDecodeError:
        from datetime import datetime

        return HeaderCheckResult(
            FileResult(file_path, FileStatus.ERROR, "Unicode decode error"), None, datetime.now().year
        )
    except Exception as e:
        from datetime import datetime

        return HeaderCheckResult(FileResult(file_path, FileStatus.ERROR, str(e)), None, datetime.now().year)


def fix_header(
    file_path: pathlib.Path,
    header_generator: Callable[[int], list[str]],
    year: int,
) -> bool:
    try:
        expected_header = header_generator(year)

        with open(file_path, encoding="utf-8") as f:
            lines = f.readlines()

        header_offset = 0
        shebang_line = []
        if lines and lines[0].startswith("#!"):
            header_offset = 1
            shebang_line = [lines[0]]

        min_lines = len(expected_header) + header_offset
        if len(lines) >= min_lines:
            header_correct = all(lines[i + header_offset] == expected_header[i] for i in range(len(expected_header)))
            if header_correct:
                return False

        content_after_header = lines[header_offset:]

        with open(file_path, "w", encoding="utf-8") as f:
            if shebang_line:
                f.writelines(shebang_line)
            f.writelines(expected_header)
            f.writelines(content_after_header)

        return True

    except (PermissionError, UnicodeDecodeError, Exception) as e:
        typer.echo(f"  {typer.style('[ERROR]', fg='yellow')} Failed to fix {file_path}: {e}")
        return False


def check_files(
    files: list[pathlib.Path], header_generator: Callable[[int], list[str]], stats: LicenseHeaderStatistics
) -> None:
    for file_path in files:
        check_result = has_correct_license_header(file_path, header_generator)
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


def fix_files(
    files: list[pathlib.Path], header_generator: Callable[[int], list[str]], stats: LicenseHeaderStatistics
) -> None:
    for file_path in files:
        check_result = has_correct_license_header(file_path, header_generator, None)
        stats.total += 1

        if check_result.result.status == FileStatus.ERROR:
            print_status("[ERROR]", "yellow", file_path, check_result.result.error or "")
            stats.errors += 1
            continue

        if check_result.result.status == FileStatus.OK:
            print_status("[SKIP]", "cyan", file_path)
            stats.record_fix(False)
        else:
            fixed = fix_header(file_path, header_generator, check_result.year)
            if fixed:
                print_status("[FIXED]", "green", file_path)
                stats.record_fix(True)
            else:
                print_status("[SKIP]", "cyan", file_path)
                stats.record_fix(False)


def check_single_file(
    file_path: pathlib.Path,
    header_generator: Callable[[int], list[str]],
    language_name: str,
    cache_manager: LicenseHeaderCacheManager,
    output_lock: threading.Lock,
) -> tuple[FileStatus, IssueType | None]:
    cached_result = cache_manager.get_cached_result(language_name, file_path)

    if cached_result:
        check_result = cached_result
        is_cached = True
    else:
        check_result = has_correct_license_header(file_path, header_generator, cache_manager)
        cache_manager.update_cache(language_name, file_path, check_result)
        is_cached = False

    with output_lock:
        if check_result.result.status == FileStatus.OK:
            tag = "[CACHED:OK]" if is_cached else "[OK]"
            print_status(tag, "green", file_path)
        elif check_result.result.status == FileStatus.ISSUE:
            if check_result.issue_type == IssueType.MISSING:
                tag = "[CACHED:MISSING]" if is_cached else "[MISSING]"
                print_status(tag, "red", file_path)
            elif check_result.issue_type == IssueType.INCORRECT:
                tag = "[CACHED:INCORRECT]" if is_cached else "[INCORRECT]"
                print_status(tag, "red", file_path)
        elif check_result.result.status == FileStatus.ERROR:
            tag = "[CACHED:ERROR]" if is_cached else "[ERROR]"
            print_status(tag, "yellow", file_path, check_result.result.error or "")

    return check_result.result.status, check_result.issue_type


def check_files_parallel(
    files: list[pathlib.Path],
    header_generator: Callable[[int], list[str]],
    language_name: str,
    stats: LicenseHeaderStatistics,
    cache_manager: LicenseHeaderCacheManager,
) -> None:
    output_lock = threading.Lock()

    with ThreadPoolExecutor(max_workers=os.cpu_count()) as executor:
        future_to_file = {
            executor.submit(
                check_single_file, file_path, header_generator, language_name, cache_manager, output_lock
            ): file_path
            for file_path in files
        }

        for future in as_completed(future_to_file):
            file_path = future_to_file[future]
            try:
                status, issue_type = future.result()

                result = FileResult(file_path, status)
                stats.record_result(result, issue_type)

            except Exception as e:
                with output_lock:
                    print_status("[ERROR]", "yellow", file_path, str(e))
                stats.total += 1
                stats.errors += 1


def fix_single_file(
    file_path: pathlib.Path,
    header_generator: Callable[[int], list[str]],
    language_name: str,
    cache_manager: LicenseHeaderCacheManager,
    output_lock: threading.Lock,
) -> str:
    cached_result = cache_manager.get_cached_result(language_name, file_path)

    if cached_result:
        check_result = cached_result
        is_cached = True
    else:
        check_result = has_correct_license_header(file_path, header_generator, cache_manager)
        cache_manager.update_cache(language_name, file_path, check_result)
        is_cached = False

    if check_result.result.status == FileStatus.ERROR:
        outcome = "error"
        with output_lock:
            print_status("[ERROR]", "yellow", file_path, check_result.result.error or "")
    elif check_result.result.status == FileStatus.OK:
        outcome = "skip"
        with output_lock:
            tag = "[CACHED:SKIP]" if is_cached else "[SKIP]"
            print_status(tag, "cyan", file_path)
    else:
        fixed = fix_header(file_path, header_generator, check_result.year)
        if fixed:
            outcome = "fixed"
            ok_result = HeaderCheckResult(FileResult(file_path, FileStatus.OK), None, check_result.year)
            cache_manager.update_cache(language_name, file_path, ok_result)
            with output_lock:
                print_status("[FIXED]", "green", file_path)
        else:
            outcome = "skip"
            with output_lock:
                print_status("[SKIP]", "cyan", file_path)

    return outcome


def fix_files_parallel(
    files: list[pathlib.Path],
    header_generator: Callable[[int], list[str]],
    language_name: str,
    stats: LicenseHeaderStatistics,
    cache_manager: LicenseHeaderCacheManager,
) -> None:
    output_lock = threading.Lock()

    with ThreadPoolExecutor(max_workers=os.cpu_count()) as executor:
        future_to_file = {
            executor.submit(
                fix_single_file, file_path, header_generator, language_name, cache_manager, output_lock
            ): file_path
            for file_path in files
        }

        for future in as_completed(future_to_file):
            file_path = future_to_file[future]
            try:
                outcome = future.result()

                stats.total += 1

                if outcome == "error":
                    stats.errors += 1
                elif outcome == "skip":
                    stats.record_fix(False)
                elif outcome == "fixed":
                    stats.record_fix(True)

            except Exception as e:
                with output_lock:
                    print_status("[ERROR]", "yellow", file_path, str(e))
                stats.total += 1
                stats.errors += 1


@check_license_headers.command()  # type: ignore[misc]
def show_headers() -> None:
    from datetime import datetime

    current_year = datetime.now().year
    for config in get_language_configs():
        header = config.header_generator(current_year)
        typer.echo(f"{config.name} License Header:")
        typer.echo("".join(header[:2]) + "\n")


@check_license_headers.command()  # type: ignore[misc]
def check(no_cache: bool = typer.Option(False, "--no-cache", help="Disable caching and re-check all files")) -> None:
    stats = LicenseHeaderStatistics()

    cache_manager = LicenseHeaderCacheManager(Files.devutils_license_headers_cache_file, enabled=not no_cache)

    for config in get_language_configs():
        files = config.collect_files()
        if files:
            check_files_parallel(files, config.header_generator, config.name, stats, cache_manager)

    cache_manager.save_cache()

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


@check_license_headers.command()  # type: ignore[misc]
def fix(no_cache: bool = typer.Option(False, "--no-cache", help="Disable caching and re-check all files")) -> None:
    stats = LicenseHeaderStatistics()

    cache_manager = LicenseHeaderCacheManager(Files.devutils_license_headers_cache_file, enabled=not no_cache)

    for config in get_language_configs():
        files = config.collect_files()
        if files:
            fix_files_parallel(files, config.header_generator, config.name, stats, cache_manager)

    cache_manager.save_cache()

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
