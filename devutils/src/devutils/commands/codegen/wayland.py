# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import shutil
import subprocess
from pathlib import Path

import typer

from devutils.constants import Directories

wayland: typer.Typer = typer.Typer()


def check_wayland_scanner_available() -> bool:
    return shutil.which("wayland-scanner") is not None


@wayland.callback(invoke_without_command=True)  # type: ignore[misc]
def main(ctx: typer.Context) -> None:
    if not check_wayland_scanner_available():
        typer.echo(
            f"{typer.style('[ERROR]', fg='red')} wayland-scanner is not available in PATH",
            err=True,
        )
        typer.echo("Install wayland-scanner using your package manager:", err=True)
        typer.echo("  - Arch/Manjaro: sudo pacman -S wayland-protocols", err=True)
        typer.echo("  - Ubuntu/Debian: sudo apt install wayland-protocols libwayland-dev", err=True)
        typer.echo("  - Fedora: sudo dnf install wayland-protocols-devel", err=True)
        raise typer.Exit(1)

    protocol_path = Path("/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml")
    if not protocol_path.exists():
        typer.echo(
            f"{typer.style('[ERROR]', fg='red')} Protocol file not found: {protocol_path}",
            err=True,
        )
        typer.echo("Make sure wayland-protocols is installed", err=True)
        raise typer.Exit(1)

    header_output = Directories.xheader_include / "xheader" / "internal" / "xdg-shell-client-protocol.h"
    source_output = Directories.xheader_source / "internal" / "xdg-shell-protocol.c"

    source_output.parent.mkdir(parents=True, exist_ok=True)
    header_output.parent.mkdir(parents=True, exist_ok=True)

    typer.echo("Generating Wayland protocol files...")

    typer.echo(f"  Generating client header: {header_output.relative_to(Directories.root)}")
    result = subprocess.run(
        [
            "wayland-scanner",
            "client-header",
            str(protocol_path),
            str(header_output),
        ],
        capture_output=True,
        text=True,
        check=False,
    )

    if result.returncode != 0:
        typer.echo(f"{typer.style('[FAIL]', fg='red')} Failed to generate client header", err=True)
        if result.stderr:
            typer.echo(result.stderr, err=True)
        raise typer.Exit(result.returncode)

    typer.echo(f"  Generating private code: {source_output.relative_to(Directories.root)}")
    result = subprocess.run(
        [
            "wayland-scanner",
            "private-code",
            str(protocol_path),
            str(source_output),
        ],
        capture_output=True,
        text=True,
        check=False,
    )

    if result.returncode != 0:
        typer.echo(f"{typer.style('[FAIL]', fg='red')} Failed to generate private code", err=True)
        if result.stderr:
            typer.echo(result.stderr, err=True)
        raise typer.Exit(result.returncode)

    typer.echo(f"{typer.style('[SUCCESS]', fg='green')} Wayland protocol files generated successfully")
