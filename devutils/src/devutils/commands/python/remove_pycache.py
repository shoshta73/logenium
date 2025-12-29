# SPDX-FileCopyrightText: 2025 Logenium Authors and Contributors
# SPDX-License-Identifier: BSD-3-Clause

import os
import shutil
import stat
from collections.abc import Callable

import typer

from devutils.constants.paths import Directories
from devutils.utils.filesystem import find_directories_by_name

remove_pycache: typer.Typer = typer.Typer()


def handle_remove_readonly(func: Callable[[str], object], path: str, exc: BaseException) -> object:
    os.chmod(path, stat.S_IWRITE)
    return func(path)


@remove_pycache.command()  # type: ignore[misc]
def run() -> None:
    typer.echo("Removing __pycache__ directories...")
    pycaches = find_directories_by_name(Directories.devutils_root, "__pycache__")
    for pycache in pycaches:
        typer.echo(f"Removing {pycache}...")
        shutil.rmtree(pycache, onexc=handle_remove_readonly)

    typer.echo("Done!")


@remove_pycache.callback(invoke_without_command=True)  # type: ignore[misc]
def main(ctx: typer.Context) -> None:
    if ctx.invoked_subcommand is None:
        run()
