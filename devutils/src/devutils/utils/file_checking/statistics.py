# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import threading
from dataclasses import dataclass

import typer

from .file_result import FileResult
from .file_status import FileStatus


@dataclass
class Statistics:
    total: int = 0
    ok: int = 0
    warnings: int = 0
    issues: int = 0
    errors: int = 0
    fixed: int = 0
    skipped: int = 0
    issue_label: str = "[ISSUE]"

    def __post_init__(self) -> None:
        self._lock = threading.Lock()

    def record_result(self, result: FileResult) -> None:
        self.total += 1
        if result.status == FileStatus.OK:
            self.ok += 1
        elif result.status == FileStatus.WARNING:
            self.warnings += 1
        elif result.status == FileStatus.ISSUE:
            self.issues += 1
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
            if self.warnings > 0:
                typer.echo(f"  {typer.style('[WARNING]', fg='yellow')}    {self.warnings}")
            if self.issues > 0:
                typer.echo(f"  {typer.style(self.issue_label, fg='red')} {self.issues}")
            if self.errors > 0:
                typer.echo(f"  {typer.style('[ERROR]', fg='red')}       {self.errors}")
        elif mode == "fix":
            typer.echo(f"  {typer.style('[FIXED]', fg='green')}    {self.fixed}")
            typer.echo(f"  {typer.style('[SKIPPED]', fg='cyan')}  {self.skipped}")
            if self.errors > 0:
                typer.echo(f"  {typer.style('[ERROR]', fg='yellow')}    {self.errors}")

        typer.echo(typer.style("=" * 60, fg="cyan"))

    def has_failures(self) -> bool:
        return self.issues > 0 or self.errors > 0

    def record_result_threadsafe(self, result: FileResult) -> None:
        with self._lock:
            self.record_result(result)

    def increment_total_threadsafe(self) -> None:
        with self._lock:
            self.total += 1

    def increment_errors_threadsafe(self) -> None:
        with self._lock:
            self.errors += 1

    def record_fix_threadsafe(self, fixed: bool) -> None:
        with self._lock:
            self.record_fix(fixed)
