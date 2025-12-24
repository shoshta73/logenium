# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import filecmp
import shutil
import subprocess
import tempfile
from pathlib import Path

import typer

from devutils.constants import Directories

stubgen = typer.Typer()


def check_stubgen_available() -> bool:
    try:
        result = subprocess.run(
            ["uv", "run", "stubgen", "--help"],
            capture_output=True,
            text=True,
            check=False,
        )
        return result.returncode == 0
    except FileNotFoundError:
        return False


@stubgen.command()
def generate(
    verbose: bool = typer.Option(
        False,
        "--verbose",
        "-v",
        help="Enable verbose output",
    ),
) -> None:
    package = "devutils"
    source_dir = Directories.devutils_source / "devutils"
    if not check_stubgen_available():
        typer.echo("Error: stubgen is not available. Make sure mypy is installed.", err=True)
        typer.echo("Run: uv pip install mypy", err=True)
        raise typer.Exit(1)

    typer.echo(f"Generating inline stubs for package '{package}'...")

    with tempfile.TemporaryDirectory() as temp_dir:
        temp_path = Path(temp_dir)

        cmd = ["uv", "run", "stubgen", "-p", package, "-o", str(temp_path)]

        if verbose:
            cmd.append("-v")

        result = subprocess.run(cmd, capture_output=True, text=True, check=False)

        if result.returncode != 0:
            if verbose:
                typer.echo(result.stdout)
                typer.echo(result.stderr, err=True)
            typer.echo(f"{typer.style('[FAIL]', fg='red')} Stub generation failed", err=True)
            raise typer.Exit(result.returncode)

        temp_stub_dir = temp_path / package
        if not temp_stub_dir.exists():
            typer.echo(f"{typer.style('[FAIL]', fg='red')} Generated stub directory not found", err=True)
            raise typer.Exit(1)

        stub_files = list(temp_stub_dir.rglob("*.pyi"))
        if not stub_files:
            typer.echo(f"{typer.style('[FAIL]', fg='red')} No stub files generated", err=True)
            raise typer.Exit(1)

        copied_count = 0
        for stub_file in stub_files:
            relative_path = stub_file.relative_to(temp_stub_dir)
            target_path = source_dir / relative_path

            target_path.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(stub_file, target_path)
            copied_count += 1

            if verbose:
                typer.echo(f"  Copied: {relative_path}")

        typer.echo(
            f"{typer.style('[SUCCESS]', fg='green')} Generated {copied_count} stub file(s) inline with source code"
        )


@stubgen.command()
def check(
    verbose: bool = typer.Option(
        False,
        "--verbose",
        "-v",
        help="Enable verbose output",
    ),
) -> None:
    package = "devutils"
    source_dir = Directories.devutils_source / "devutils"
    if not check_stubgen_available():
        typer.echo("Error: stubgen is not available. Make sure mypy is installed.", err=True)
        typer.echo("Run: uv pip install mypy", err=True)
        raise typer.Exit(1)

    typer.echo(f"Checking inline stubs for package '{package}'...")

    if not source_dir.exists():
        typer.echo(f"{typer.style('[FAIL]', fg='red')} Source directory does not exist: {source_dir}", err=True)
        raise typer.Exit(1)

    existing_stubs = list(source_dir.rglob("*.pyi"))
    if not existing_stubs:
        typer.echo(f"{typer.style('[FAIL]', fg='red')} No stub files found in source directory", err=True)
        typer.echo("Run 'devutils python stubgen generate' to generate stubs")
        raise typer.Exit(1)

    with tempfile.TemporaryDirectory() as temp_dir:
        temp_path = Path(temp_dir)
        typer.echo("Generating fresh stubs for comparison...")

        cmd = ["uv", "run", "stubgen", "-p", package, "-o", str(temp_path)]
        result = subprocess.run(cmd, capture_output=True, text=True, check=False)

        if result.returncode != 0:
            typer.echo(f"{typer.style('[FAIL]', fg='red')} Failed to generate temporary stubs for comparison", err=True)
            raise typer.Exit(result.returncode)

        temp_stub_dir = temp_path / package
        if not temp_stub_dir.exists():
            typer.echo(f"{typer.style('[FAIL]', fg='red')} Generated stub directory not found", err=True)
            raise typer.Exit(1)

        fresh_stubs = list(temp_stub_dir.rglob("*.pyi"))
        differences = []

        existing_stub_set = {f.relative_to(source_dir) for f in existing_stubs}
        fresh_stub_set = {f.relative_to(temp_stub_dir) for f in fresh_stubs}

        missing_stubs = fresh_stub_set - existing_stub_set
        extra_stubs = existing_stub_set - fresh_stub_set

        for rel_path in missing_stubs:
            differences.append(f"  Missing: {rel_path}")

        for rel_path in extra_stubs:
            differences.append(f"  Extra: {rel_path}")

        for rel_path in existing_stub_set & fresh_stub_set:
            existing_file = source_dir / rel_path
            fresh_file = temp_stub_dir / rel_path
            if not filecmp.cmp(existing_file, fresh_file, shallow=False):
                differences.append(f"  Out of date: {rel_path}")

        if differences:
            typer.echo(f"{typer.style('[FAIL]', fg='red')} Stub files are not up to date:", err=True)
            for diff in differences:
                typer.echo(diff, err=True)
            typer.echo("\nRun 'devutils python stubgen generate' to update stubs")
            raise typer.Exit(1)

        typer.echo(f"{typer.style('[SUCCESS]', fg='green')} All {len(existing_stubs)} stub file(s) are up to date")
